using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Management;
using System.Management.Instrumentation;
using System.Net;
using System.Net.Sockets;
using System.Globalization;
using CSUtils;


namespace LampAdmin
{
    public partial class FormMain : Form
    {
        private static CultureInfo ci = new CultureInfo("en-US");

        /// <summary>
        /// Parse parameters, too.
        /// </summary>
        /// <param name="s"></param>
        /// <returns></returns>
        private static Dictionary<int, string> ParseRecord(string s)
        {
            Dictionary<int, string> r = new Dictionary<int, string>();

            for (int so_far = 0; so_far < s.Length; )
            {
                int eqpos = s.IndexOf('=', so_far);
                if (eqpos < 0)
                {
                    break;
                }

                int startpos = eqpos + 1;
                string key = s.Substring(so_far, eqpos - so_far);
                int next_so_far = startpos;
                string val = "";

                if (s.Length > startpos)
                {
                    if (s[startpos] == '"')
                    {
                        int endpos = s.IndexOf('"', startpos + 1);
                        if (endpos < 0)
                        {
                            // error.
                            throw new Exception(String.Format("parse_record: unmatched \" for key {0} in record {1}", key, s));
                        }
                        else
                        {
                            val = s.Substring(startpos + 1, endpos - startpos - 1);
                            next_so_far = endpos + 1;
                        }
                    }
                    else
                    {
                        int endpos = s.IndexOf(' ', startpos);
                        if (endpos < 0)
                        {
                            val = s.Substring(startpos);
                            next_so_far = s.Length - 1;
                        }
                        else
                        {
                            val = s.Substring(startpos, endpos - startpos);
                            next_so_far = endpos + 1;
                        }
                    }
                }
                r[int.Parse(key)] = val;
                while (next_so_far < s.Length && Char.IsWhiteSpace(s[next_so_far]))
                {
                    ++next_so_far;
                }
                so_far = next_so_far;
            }
            return r;
        }

        /// <summary>
        /// SQL table in the server.
        /// </summary>
        private class Table
        {
            private int RowIndex_ = 0;

            /// <summary>
            /// Table contents.
            /// </summary>
            public string[,] Rows = null;

            /// <summary>
            /// Is table complete ?
            /// </summary>
            public bool IsComplete
            {
                get { return Rows != null && RowIndex_>=Rows.GetLength(0);  }
            }

            /// <summary>
            /// Feed some SQL COMMANDs.
            /// </summary>
            /// <param name="cmd">"SQL"</param>
            /// <param name="op">Either "OK" or "ROW"</param>
            /// <param name="parameters">Fields</param>
            public void Feed(string cmd, string op, string parameters)
            {
                Dictionary<int, string> fields = ParseRecord(parameters);
                if (op == "OK")
                {
                    // Get the number of rows.
                    int nrows = int.Parse(fields[1]);
                    int nfields = int.Parse(fields[3]);
                    if (nrows == 0)
                    {
                        Rows = new string[0,0];
                    } else {
                        Rows = new string[nrows,nfields];
                    }
                }
                else if (op == "ROW")
                {
                    if (RowIndex_ < Rows.GetLength(0))
                    {
                        int nfields = Rows.GetLength(1);
                        for (int i = 0; i < nfields; ++i)
                        {
                            Rows[RowIndex_, i] = fields[i + 1];
                        }
                    }
                    ++RowIndex_;
                }
                else
                {
                    throw new Exception("Invalid parameters to Feed!");
                }
            }
        }

        // Split string into two by whitespace.
        private static void Split2(string s, out string first, out string second)
        {
            int space_idx = s.IndexOf(' ');
            if (space_idx >= 0)
            {
                first = s.Substring(0, space_idx);
                int next_idx = space_idx + 1;
                while (next_idx < s.Length && s[next_idx] == ' ')
                {
                    ++next_idx;
                }
                second = s.Substring(next_idx, s.Length - next_idx);
            }
            else
            {
                first = s;
                second = "";
            }
        }

        /// <summary>
        /// Lamp representation :)
        /// </summary>
        class Lamp
        {
            // Table "Lamp"
            public int id;
            public bool IsPublic;
            public string IMEI;
            public string Firmware_Name;
            public string Firmware_Version;
            public string Firmware_Build_date;
            public string Phone_no;
            public int Reg_no;
            public string Description;
            public string Owner;
            public string Last_Contact;
            public string Online;
            public bool KeepDown;
            // Table "LampConfiguration".
            public string AdministratorID;
            public string SettingTime;
            public double LampVoltage;
            public string Blinking_Pattern;
            public bool Blinking_Enabled;
            public double Battery_Voltage_Min;
            public double Battery_Hysteresis;
            public double Photocell_Threshold;
            public string Serial_No;
            public double Charger_CutIn_Voltage;
            public double Charger_CutOut_Voltage;

            public static string TableLampQuery =
                "SELECT " +
                "id, IsPublic, IMEI, Firmware_name, Firmware_Version, Firmware_Build_date, " +
                "Phone_no, Reg_no, Description, Owner, " +
                "Last_Contact, Online, KeepDown from Lamp order by id asc";

            public string TableLampConfigurationQuery()
            {
                return "SELECT " +
                    "AdministratorID, SettingTime, " +
                    "LampVoltage, Blinking_Pattern, Blinking_Enabled, " +
                    "Battery_Voltage_Min, Battery_Hysteresis, " +
                    "Photocell_Threshold, " +
                    "Serial_No, " +
                    "Charger_CutIn_Voltage, " +
                    "Charger_CutOut_Voltage " +
                "FROM LampConfiguration where LampID=" + id + " ORDER BY SettingTime Desc LIMIT 1";
            }

            private static string escape_sql(string s)
            {
                StringBuilder sb = new StringBuilder();
                foreach (char c in s)
                {
                    switch (c)
                    {
                        case '\'':
                            sb.Append("\\'");
                            break;
                        case '"':
                            sb.Append("\\\"");
                            break;
                        case '\\':
                            sb.Append("\\\\");
                            break;
                        default:
                            sb.Append(c);
                            break;
                    }
                }
                return sb.ToString();
            }

            private static string strip_invalid(string s)
            {
                StringBuilder sb = new StringBuilder();
                foreach (char c in s)
                {
                    switch (c)
                    {
                        case '\'':
                            break;
                        case '"':
                            break;
                        case '\\':
                            break;
                        default:
                            sb.Append(c);
                            break;
                    }
                }
                return sb.ToString();
            }

            public string UpdateQuery()
            {
                return "SQL QUERY " +
                    "UPDATE Lamp SET " +
                    "IsPublic='" + (IsPublic ? "Y" : "N") + "', " +
                        "Phone_no='" + escape_sql(Phone_no) + "', " +
                        "Reg_no=" + Reg_no + ", " +
                        "Description='" + escape_sql(Description) + "', " +
                        "Owner='" + escape_sql(Owner) + "', " +
                        "KeepDown='" + (KeepDown ? "Y" : "N") + "' "+
                        "WHERE id=" + id;
            }

            private string field_entry(int field_no, string value)
            {
                return " " + field_no.ToString(ci) + "=" + strip_invalid(value);
            }

            public string ConfigurationQuery()
            {
                return "forward " + id + " config =" +
                    field_entry(1, LampVoltage.ToString(ci)) +
                    field_entry(2, Blinking_Pattern) +
                    field_entry(3, Blinking_Enabled ? "Y" : "N") +
                    field_entry(9, Battery_Voltage_Min.ToString(ci)) +
                    field_entry(10, Battery_Hysteresis.ToString(ci)) +
                    field_entry(11, Photocell_Threshold.ToString(ci)) +
                    field_entry(21, Charger_CutIn_Voltage.ToString(ci)) +
                    field_entry(22, Charger_CutOut_Voltage.ToString(ci));
            }

            /// <summary>
            /// Fetch fields from the tables.
            /// </summary>
            /// <param name="LampTable"></param>
            /// <param name="RowIndex"></param>
            public void FetchInfoFields(Table LampTable, int RowIndex)
            {
                // "Lamp".
                    string[,] rows = LampTable.Rows;
                    id = int.Parse(rows[RowIndex, 0]);
                    IsPublic = rows[RowIndex, 1] == "Y";
                    IMEI = rows[RowIndex, 2];
                    Firmware_Name = rows[RowIndex, 3];
                    Firmware_Version = rows[RowIndex, 4];
                    Firmware_Build_date = rows[RowIndex, 5];
                    Phone_no = rows[RowIndex, 6];
                    Reg_no = int.Parse(rows[RowIndex, 7]);
                    Description = rows[RowIndex, 8];
                    Owner = rows[RowIndex, 9];
                    Last_Contact = rows[RowIndex, 10];
                    Online = rows[RowIndex, 11];
                    KeepDown = rows[RowIndex, 12] == "Y";
            }

            public void FetchConfigurationFields(Table ConfigurationTable)
            {
                    string[,] rows = ConfigurationTable.Rows;
                    AdministratorID = rows[0, 0];
                    SettingTime = rows[0, 1];
                    LampVoltage = double.Parse(rows[0, 2], ci);
                    Blinking_Pattern = rows[0, 3];
                    Blinking_Enabled = rows[0, 4] == "Y" ? true : false;
                    Battery_Voltage_Min = double.Parse(rows[0, 5], ci);
                    Battery_Hysteresis = double.Parse(rows[0, 6], ci);
                    Photocell_Threshold = double.Parse(rows[0, 7], ci);
                    Serial_No = rows[0, 8];
                    Charger_CutIn_Voltage = double.Parse(rows[0, 9], ci);
                    Charger_CutOut_Voltage = double.Parse(rows[0, 10], ci);
            }

            public Lamp Clone()
            {
                return MemberwiseClone() as Lamp;
            }
        } // class Lamp.

        /// <summary>
        /// Pair of current and new values.
        /// </summary>
        class LampPair
        {
            public Lamp Current;
            public Lamp New;

            public LampPair(Lamp Current, Lamp New)
            {
                this.Current = Current;
                this.New = New;
            }
        } // class LampPair;

        private void LampToScreen(LampPair lamps)
        {
            {
                Lamp lamp = lamps.Current;
                textBoxOldID.Text = lamp.id.ToString();
                checkBoxOldIsPublic.Checked = lamp.IsPublic;
                textBoxOldIMEI.Text = lamp.IMEI;
                textBoxOldPhoneNo.Text = lamp.Phone_no;
                textBoxOldSerialNo.Text = lamp.Serial_No;
                textBoxOldLampVoltage.Text = lamp.LampVoltage.ToString(ci);
                textBoxOldBlinkingPattern.Text = lamp.Blinking_Pattern;
                checkBoxOldBlinkingEnabled.Checked = lamp.Blinking_Enabled;
                textBoxOldBatteryVoltageMin.Text = lamp.Battery_Voltage_Min.ToString(ci);
                textBoxOldBatteryHysteresis.Text = lamp.Battery_Hysteresis.ToString(ci);
                textBoxOldPhotocellThreshold.Text = lamp.Photocell_Threshold.ToString(ci);
                textBoxOldChargerCutInVoltage.Text = lamp.Charger_CutIn_Voltage.ToString(ci);
                textBoxOldChargerCutOutVoltage.Text = lamp.Charger_CutOut_Voltage.ToString(ci);
                checkBoxOldKeepOffline.Checked = lamp.KeepDown;
            }

            {
                Lamp lamp = lamps.New;
                checkBoxNewIsPublic.Checked = lamp.IsPublic;
                textBoxNewIMEI.Text = lamp.IMEI;
                textBoxNewPhoneNo.Text = lamp.Phone_no;
                textBoxNewLampVoltage.Text = lamp.LampVoltage.ToString(ci);
                textBoxNewBlinkingPattern.Text = lamp.Blinking_Pattern;
                checkBoxNewBlinkingEnabled.Checked = lamp.Blinking_Enabled;
                textBoxNewBatteryVoltageMin.Text = lamp.Battery_Voltage_Min.ToString(ci);
                textBoxNewBatteryHysteresis.Text = lamp.Battery_Hysteresis.ToString(ci);
                textBoxNewPhotocellThreshold.Text = lamp.Photocell_Threshold.ToString(ci);
                textBoxNewChargerCutInVoltage.Text = lamp.Charger_CutIn_Voltage.ToString(ci);
                textBoxNewChargerCutOutVoltage.Text = lamp.Charger_CutOut_Voltage.ToString(ci);
                checkBoxNewKeepOffline.Checked = lamp.KeepDown;
            }
        }

        private void ScreenToLamp(LampPair lamps)
        {
            Lamp lamp = lamps.New;

            lamp.id = int.Parse(textBoxOldID.Text);
            lamp.IsPublic = checkBoxNewIsPublic.Checked;
            lamp.IMEI = textBoxNewIMEI.Text;
            lamp.Phone_no = textBoxNewPhoneNo.Text;
            lamp.Serial_No = textBoxOldSerialNo.Text;
            lamp.LampVoltage = double.Parse(textBoxNewLampVoltage.Text, ci);
            lamp.Blinking_Pattern = textBoxNewBlinkingPattern.Text;
            lamp.Blinking_Enabled = checkBoxNewBlinkingEnabled.Checked;
            lamp.Battery_Voltage_Min = double.Parse(textBoxNewBatteryVoltageMin.Text, ci);
            lamp.Battery_Hysteresis = double.Parse(textBoxNewBatteryHysteresis.Text, ci);
            lamp.Photocell_Threshold = double.Parse(textBoxNewPhotocellThreshold.Text, ci);
            lamp.Charger_CutIn_Voltage = double.Parse(textBoxNewChargerCutInVoltage.Text, ci);
            lamp.Charger_CutOut_Voltage = double.Parse(textBoxNewChargerCutOutVoltage.Text, ci);
            lamp.KeepDown = checkBoxNewKeepOffline.Checked;
        }

        private delegate void InvokeDelegate();
        private FormConnection formConnection = null;
        private CMRAssembler packet_assembler_ = null;
        private List<Table> Tables_ = new List<Table>();
        private Lamp[] Lamps_ = null;
        private LampPair[] LampPairs_ = null;
        private int CurrentLampIndex_ = 0;

        public void SetCurrentLampIndex(int NewIndex)
        {
            if (NewIndex < 0)
            {
                NewIndex = 0;
            }
            if (NewIndex >= LampPairs_.Length)
            {
                NewIndex = LampPairs_.Length - 1;
            }
            if (NewIndex>=0 && NewIndex<LampPairs_.Length && NewIndex != CurrentLampIndex_)
            {
                // Swap screen contents, if possible.
                try
                {
                    ScreenToLamp(LampPairs_[CurrentLampIndex_]);
                    LampToScreen(LampPairs_[NewIndex]);
                    CurrentLampIndex_ = NewIndex;
                    textBoxNavCurrent.Text = (CurrentLampIndex_ + 1).ToString() + " of " + LampPairs_.Length;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Please check input", "Error");
                }
            }
        }

        public FormMain()
        {
            InitializeComponent();
            
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            simpleTcpClient.Close();
            Application.Exit();
        }

        /// <summary>
        /// Append a line to the textbox log.
        /// </summary>
        /// <param name="prefix">Prefix string.</param>
        /// <param name="message">Message string.</param>
        private void DoLog(string prefix, string message)
        {
            string text_to_log = prefix + ":" + message + "\r\n";
            if (InvokeRequired)
            {
                Invoke(new InvokeDelegate(
                    delegate()
                    {
                       textBoxLog.AppendText(text_to_log);
                    }));
            }
            else
            {
                textBoxLog.AppendText(text_to_log);
            }
        }

        /// <summary>
        /// Send message down the throat to the server.
        /// </summary>
        /// <param name="message"></param>
        private void Emit(string message)
        {
            DoLog("SENT", message);
            simpleTcpClient.Write(CMR.ToBytes(CMR.LAMPNET, message));
        }

        /// <summary>
        /// Show server connection dialogue.
        /// </summary>
        private void DoConnect()
        {
            if (formConnection == null)
            {
                formConnection = new FormConnection();
            }
            FormConnection.Settings server_settings = formConnection.DoShowDialog(this);
            if (server_settings != null)
            {
                // Do connection.
                try
                {
                    packet_assembler_ = new CMRAssembler();
                    simpleTcpClient.Connect(String.Format("{0}:{1}", server_settings.ServerName, server_settings.ServerPort));
                }
                catch (Exception)
                {
                    MessageBox.Show(String.Format("Failed to connect server {0}:{1}", server_settings.ServerName, server_settings.ServerPort), "Error");
                    return;
                }

                // Send login info.
                try
                {
                    string login_msg = String.Format("admin = 1={0} 2={1}", server_settings.Username, server_settings.Password);
                    Emit(login_msg);
                    Emit("SQL QUERY " + Lamp.TableLampQuery);
                }
                catch (Exception)
                {
                    simpleTcpClient.Close();
                    return;
                }
            }
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            DoConnect();
        }

        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DoConnect();
        }

        private void DoDataRead(byte[] buffer, int count)
        {
            packet_assembler_.Feed(buffer, count);
            for (CMR cmr = packet_assembler_.Pop(); cmr != null; cmr = packet_assembler_.Pop())
            {
                string packet = cmr.ToString();
                DoLog("Read", packet);
                if (cmr.type == CMR.PING)
                {
                    simpleTcpClient.Write(CMR.ToBytes(CMR.PING, ""));
                }
                else if (cmr.type == CMR.LAMPNET)
                {
                    try
                    {
                        // Chop off first two.
                        string cmd;
                        string op;
                        string parameters;
                        {
                            string op_plus_parameters;
                            Split2(cmr.ToString(), out cmd, out op_plus_parameters);
                            Split2(op_plus_parameters, out op, out parameters);
                        }
                        if (cmd == "ADMIN")
                        {
                            if (op == "OK")
                            {
                                // pass, login OK.
                                DoLog("LOGIN", "OK");
                            }
                            else if (op == "ERROR")
                            {
                                MessageBox.Show("Failed to log in", "Error");
                            }
                            else
                            {
                                DoLog("Error", "Unknown ADMIN response");
                            }
                        }
                        else if (cmd == "SQL")
                        {
                            // Dude, check stuff out.
                            if (Tables_.Count == 0 || Tables_[Tables_.Count-1].IsComplete)
                            {
                                Tables_.Add(new Table());
                            }
                            Table tbl = Tables_[Tables_.Count - 1];
                            tbl.Feed(cmd, op, parameters);
                            if (tbl.IsComplete) {
                                DoLog("debug", "Table filled.");
                            }

                            // Initialization sequence.
                            if (tbl.IsComplete)
                            {
                                if (Tables_.Count == 1)
                                {
                                    // Send out SQL queries.
                                    int n = tbl.Rows.GetLength(0);
                                    Lamps_ = new Lamp[n];
                                    for (int i = 0; i < n; ++i)
                                    {
                                        Lamps_[i] = new Lamp();
                                        Lamps_[i].FetchInfoFields(tbl, i);
                                        Emit("SQL QUERY " + Lamps_[i].TableLampConfigurationQuery());
                                    }
                                }
                                if (Tables_.Count == Tables_[0].Rows.GetLength(0) + 1)
                                {
                                    // Gotcha, all complete.
                                    int n = Tables_[0].Rows.GetLength(0);
                                    LampPairs_ = new LampPair[n];
                                    for (int i = 0; i < n; ++i)
                                    {
                                        Lamp lamp = Lamps_[i];
                                        if (Tables_[i + 1].Rows.Length > 0)
                                        {
                                            lamp.FetchConfigurationFields(Tables_[i + 1]);
                                        }
                                        LampPairs_[i] = new LampPair(lamp.Clone(), lamp.Clone());
                                    }
                                    DoLog("debug", "complete");
                                    LampToScreen(LampPairs_[0]);
                                    CurrentLampIndex_ = 0;
                                    textBoxNavCurrent.Text = (CurrentLampIndex_ + 1).ToString() + " of " + LampPairs_.Length;
                                }
                            }
                        }
                        else
                        {
                            DoLog("Error", "Unknown command:"+cmd);
                        }
                    }
                    catch (Exception ex)
                    {
                        DoLog("Exception", ex.ToString());
                    }
                }
            }
        }

        private void simpleTcpClient_DataRead(byte[] buffer, int count)
        {
            if (this.InvokeRequired)
            {
                Invoke(new InvokeDelegate(
                    delegate()
                    {
                        DoDataRead(buffer, count);
                    }));
            }
            else
            {
                DoDataRead(buffer, count);
            }
        }

        private void simpleTcpClient_ExceptionHandled(SocketException ex)
        {
            MessageBox.Show("Oops, connection lost.", "Error");
            // fixme: do what?
        }

        private void simpleTcpClient_StatusChanged(CSUtils.SimpleTcpClient.STATUS status)
        {
            // fixme: do what?
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            Emit(textBoxSend.Text);
        }

        private void buttonNavFirst_Click(object sender, EventArgs e)
        {
            SetCurrentLampIndex(0);
        }

        private void buttonNavPrevious_Click(object sender, EventArgs e)
        {
            SetCurrentLampIndex(CurrentLampIndex_ - 1);
        }

        private void buttonNavNext_Click(object sender, EventArgs e)
        {
            SetCurrentLampIndex(CurrentLampIndex_ + 1);
        }

        private void buttonNavLast_Click(object sender, EventArgs e)
        {
            SetCurrentLampIndex(LampPairs_.Length - 1);
        }

        private void buttonUploadNew_Click(object sender, EventArgs e)
        {
            // SQL command first :)
            LampPair lamppair = LampPairs_[CurrentLampIndex_];
            try
            {
                ScreenToLamp(lamppair);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Check input fields", "Error");
                return;
            }
            Lamp lamp = lamppair.New;
            Emit(lamp.UpdateQuery());
            Emit(lamp.ConfigurationQuery());

            // Forward to the client.
        }

        private void buttonShutdown_Click(object sender, EventArgs e)
        {
            LampPair lamppair = LampPairs_[CurrentLampIndex_];
            string cmd = "forward " + lamppair.Current.id + " shutdown";
            Emit(cmd);
        }

        private void buttonStayOnline_Click(object sender, EventArgs e)
        {
            LampPair lamppair = LampPairs_[CurrentLampIndex_];
            string cmd = "SQL QUERY Update Lamp SET WakeUp='Y' WHERE id=" + lamppair.Current.id;
            Emit(cmd);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            LampPair lamppair = LampPairs_[CurrentLampIndex_];
            string cmd = "SQL QUERY Update Lamp SET WakeUp='N' WHERE id=" + lamppair.Current.id;
            Emit(cmd);
        }

        private void buttonSendAllRefreshSensors_Click(object sender, EventArgs e)
        {
            foreach (LampPair lp in LampPairs_)
            {
                string cmd = "forward " + lp.Current.id + " sensorcounter ?";
                Emit(cmd);
            }
        }

        private void buttonLight7Send_Click(object sender, EventArgs e)
        {
            string cmd = "forward 7 powerlight = 1=" +
                textBoxLight7Current.Text +
                " 2=" +
                textBoxLight7Default.Text;
            Emit(cmd);
        }

        private void buttonLight8Send_Click(object sender, EventArgs e)
        {
            string cmd = "forward 8 powerlight = 1=" +
                textBoxLight8Current.Text +
                " 2=" +
                textBoxLight8Default.Text;
            Emit(cmd);
        }
    }
}