using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CSUtils;

namespace LampEmulator
{
    public partial class MainForm : Form
    {
        private LineAssembler la_ = new LineAssembler(new char[] { '\r' });
        private List<MemoryAreaControl> memory_areas_ = new List<MemoryAreaControl>();
        private byte[] memory_ = new byte[65536];

        private void DoUpdateMemory()
        {
            foreach (MemoryAreaControl ma in memory_areas_)
            {
                int addr = ma.MemoryAddress;
                int offset = 0;
                string sbuffer = ma.MemoryData;
                StringBuilder sb = new StringBuilder();
                int i;
                for (i = 0; i < sbuffer.Length; ++i)
                {
                    char c = sbuffer[i];
                    if (Char.IsDigit(c) || "abcdefABCDEF".IndexOf(c) >= 0)
                    {
                        sb.Append(c);
                        if (sb.Length == 2)
                        {
                            byte data = byte.Parse(sb.ToString(), System.Globalization.NumberStyles.AllowHexSpecifier);
                            memory_[addr + offset] = data;
                            ++offset;
                            sb.Remove(0, sb.Length);
                        }
                    }
                }
            }
        }

        public MainForm()
        {
            InitializeComponent();
            memory_areas_.Add(memoryAreaControl1);
            memory_areas_.Add(memoryAreaControl2);
            memory_areas_.Add(memoryAreaControl3);
            memory_areas_.Add(memoryAreaControl4);
            memory_areas_.Add(memoryAreaControl5);
            memory_areas_.Add(memoryAreaControl6);
            memory_areas_.Add(memoryAreaControl7);
            memory_areas_.Add(memoryAreaControl8);

            for (int i = 0; i < memory_.Length; ++i)
            {
                memory_[i] = 0x00;
            }
            DoUpdateMemory();
        }

        private delegate void InvokeDelegate();

        private void Log(string text)
        {
            string text_to_log = text + "\r\n";
            if (textBoxLog.InvokeRequired)
            {
                Invoke(new InvokeDelegate(delegate() { textBoxLog.AppendText(text_to_log); }));
            }
            else
            {
                textBoxLog.AppendText(text_to_log);
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void timerStartup_Tick(object sender, EventArgs e)
        {
            timerStartup.Enabled = false;
            try
            {
                serialPortLine.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error");
            }
        }

        private void timerBlinking_Tick(object sender, EventArgs e)
        {
            // Update blinking info...
            if (serialPortLine.IsOpen && checkBoxBlinking.Checked)
            {
                try
                {
                    serialPortLine.Write(new byte[] { 00 });
                }
                catch (Exception)
                {
                    Log("Plinker send 0x00 failure.");
                }
            }

            // Update current time.
            if (checkBoxKeepTimeCurrent.Checked)
            {
                System.DateTime dt = System.DateTime.Now;
                textBoxTResponseData.Text = String.Format("{0:d4}{1:d2}{2:d2}{3:d2}{4:d2}{5:d2}",
                    dt.Year, dt.Second, dt.Minute, dt.Hour, dt.Day, dt.Month);
            }
        }

        private void serialPortLine_LineRead(string line)
        {
            if (line.Length > 0)
            {
                Log("Query:" + line);
                switch (line[0])
                {
                    case 'R':
                        try {
                            int addr = int.Parse(line.Substring(1, 4), System.Globalization.NumberStyles.HexNumber);
                            int length = int.Parse(line.Substring(5, 2), System.Globalization.NumberStyles.HexNumber);

                            // Build response :)
                            StringBuilder sb = new StringBuilder();
                            int checksum = 0x00;
                            sb.AppendFormat("{0:X4}", addr);
                            for (int i=0; i<length; ++i) {
                                byte data = memory_[addr + i];
                                checksum = checksum + data;
                                sb.AppendFormat("{0:X2}", data);
                            }
                            sb.AppendFormat("{0:X2}", (checksum & 0xFF));

                            string response = sb.ToString();
                            serialPortLine.Writeln(response);
                            Log("Response: " + response);
                        }
                        catch (Exception ex)
                        {
                            Log("Error: " + ex.ToString());
                        }
                        break;
                    case 'W':
                        try
                        {
                            int addr = int.Parse(line.Substring(1, 4), System.Globalization.NumberStyles.HexNumber);
                            int length = int.Parse(line.Substring(5, 2), System.Globalization.NumberStyles.HexNumber);
                            for (int i = 0; i < length; ++i)
                            {
                                byte data = byte.Parse(line.Substring(7+2*i, 2), System.Globalization.NumberStyles.HexNumber);
                                memory_[addr + i] = data;
                            }
                            serialPortLine.Write("OK");
                            Log("Response: OK");
                        }
                        catch (Exception ex)
                        {
                            Log("Error: " + ex.ToString());
                        }
                        break;
                    case 'T':
                        {
                            string t = textBoxTResponseData.Text;
                            serialPortLine.Write(t);
                            Log("Response:" + t);
                        }
                        break;
                    case 'S':
                        try {
                            // SYYYY00mmhhDDMM
                            int year = int.Parse(line.Substring(1, 4));
                            int minutes = int.Parse(line.Substring(7, 2));
                            int hours = int.Parse(line.Substring(9, 2));
                            int day = int.Parse(line.Substring(11, 2));
                            int month = int.Parse(line.Substring(13, 2));
                            serialPortLine.Writeln("OK");
                            Log("Response: OK");
                        }
                        catch (Exception ex)
                        {
                            Log("Error: " + ex.ToString());
                        }
                        break;
                }
            }
        }

        private void buttonUpdateMemory_Click(object sender, EventArgs e)
        {
            // Update memory according to controls.
            DoUpdateMemory();
        }

        private void buttonUpdateControls_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();
            foreach (MemoryAreaControl ma in memory_areas_)
            {
                sb.Remove(0, sb.Length);
                for (int i = 0; i < ma.MemoryLength; ++i)
                {
                    byte data = memory_[ma.MemoryAddress + i];
                    if (i>0)
                    {
                        sb.Append(" ");
                    }
                    sb.AppendFormat("{0:X2}", data);
                }
                ma.MemoryData = sb.ToString();
            }
        }

        private void buttonLogClear_Click(object sender, EventArgs e)
        {
            textBoxLog.Text = "";
        }

        private static string InsertSpaces(string s)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < s.Length; ++i)
            {
                if (i>0 && ((i % 2) == 0)) {
                    sb.Append(' ');
                }
                sb.Append(s[i]);
            }
            return sb.ToString();
        }

        private void buttonParseServerMemoryLog_Click(object sender, EventArgs e)
        {
            try {
                // Filter bad characters out.
                string text = CSUtils.Misc.Filter(textBoxServerMemoryLog.Text, "MEMORY=:;0123456789ABCDEF");
                string memorykey = "MEMORY=";
                int start = text.IndexOf(memorykey);
                if (start < 0)
                {
                    throw new ApplicationException("Invalid memory log.");
                }
                string smemory = text.Substring(start + memorykey.Length);
                string[] memory_areas = smemory.Split(new char[] { ';' });
                foreach (string area in memory_areas)
                {
                    string[] keyval = area.Split(new char[] { ':' });
                    if (keyval.Length >= 2)
                    {
                        int addr = int.Parse(keyval[0], System.Globalization.NumberStyles.HexNumber);
                        foreach (MemoryAreaControl mc in memory_areas_)
                        {
                            if (mc.MemoryAddress == addr)
                            {
                                mc.MemoryData = InsertSpaces(keyval[1]);
                            }
                        }
                    }
                }
                DoUpdateMemory();
            } catch (Exception ex) {
                MessageBox.Show(ex.ToString(), "Error");
            }
        }
    }
}