namespace LampEmulator
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.timerStartup = new System.Windows.Forms.Timer(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxTResponseData = new System.Windows.Forms.TextBox();
            this.timerBlinking = new System.Windows.Forms.Timer(this.components);
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.checkBoxKeepTimeCurrent = new System.Windows.Forms.CheckBox();
            this.buttonLogClear = new System.Windows.Forms.Button();
            this.checkBoxBlinking = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxLog = new System.Windows.Forms.TextBox();
            this.serialPortLine = new CSUtils.SerialPortLine();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.buttonParseServerMemoryLog = new System.Windows.Forms.Button();
            this.textBoxServerMemoryLog = new System.Windows.Forms.TextBox();
            this.buttonUpdateControls = new System.Windows.Forms.Button();
            this.buttonUpdateMemory = new System.Windows.Forms.Button();
            this.memoryAreaControl8 = new LampEmulator.MemoryAreaControl();
            this.memoryAreaControl5 = new LampEmulator.MemoryAreaControl();
            this.memoryAreaControl6 = new LampEmulator.MemoryAreaControl();
            this.memoryAreaControl7 = new LampEmulator.MemoryAreaControl();
            this.memoryAreaControl2 = new LampEmulator.MemoryAreaControl();
            this.memoryAreaControl1 = new LampEmulator.MemoryAreaControl();
            this.memoryAreaControl4 = new LampEmulator.MemoryAreaControl();
            this.memoryAreaControl3 = new LampEmulator.MemoryAreaControl();
            this.menuStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(936, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(103, 22);
            this.exitToolStripMenuItem.Text = "&Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // timerStartup
            // 
            this.timerStartup.Enabled = true;
            this.timerStartup.Tick += new System.EventHandler(this.timerStartup_Tick);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.textBoxTResponseData);
            this.groupBox1.Location = new System.Drawing.Point(597, 103);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(185, 97);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "TResponse";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 30);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(33, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Data:";
            // 
            // textBoxTResponseData
            // 
            this.textBoxTResponseData.Location = new System.Drawing.Point(45, 27);
            this.textBoxTResponseData.Name = "textBoxTResponseData";
            this.textBoxTResponseData.Size = new System.Drawing.Size(128, 20);
            this.textBoxTResponseData.TabIndex = 0;
            this.textBoxTResponseData.Text = "20060002210710";
            // 
            // timerBlinking
            // 
            this.timerBlinking.Enabled = true;
            this.timerBlinking.Interval = 1000;
            this.timerBlinking.Tick += new System.EventHandler(this.timerBlinking_Tick);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(0, 27);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(936, 536);
            this.tabControl1.TabIndex = 13;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.checkBoxKeepTimeCurrent);
            this.tabPage1.Controls.Add(this.buttonLogClear);
            this.tabPage1.Controls.Add(this.checkBoxBlinking);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.textBoxLog);
            this.tabPage1.Controls.Add(this.serialPortLine);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(928, 510);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Communications";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // checkBoxKeepTimeCurrent
            // 
            this.checkBoxKeepTimeCurrent.AutoSize = true;
            this.checkBoxKeepTimeCurrent.Checked = true;
            this.checkBoxKeepTimeCurrent.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxKeepTimeCurrent.Location = new System.Drawing.Point(8, 147);
            this.checkBoxKeepTimeCurrent.Name = "checkBoxKeepTimeCurrent";
            this.checkBoxKeepTimeCurrent.Size = new System.Drawing.Size(109, 17);
            this.checkBoxKeepTimeCurrent.TabIndex = 17;
            this.checkBoxKeepTimeCurrent.Text = "Keep time current";
            this.checkBoxKeepTimeCurrent.UseVisualStyleBackColor = true;
            // 
            // buttonLogClear
            // 
            this.buttonLogClear.Location = new System.Drawing.Point(259, 8);
            this.buttonLogClear.Name = "buttonLogClear";
            this.buttonLogClear.Size = new System.Drawing.Size(75, 23);
            this.buttonLogClear.TabIndex = 16;
            this.buttonLogClear.Text = "Clear";
            this.buttonLogClear.UseVisualStyleBackColor = true;
            this.buttonLogClear.Click += new System.EventHandler(this.buttonLogClear_Click);
            // 
            // checkBoxBlinking
            // 
            this.checkBoxBlinking.AutoSize = true;
            this.checkBoxBlinking.Location = new System.Drawing.Point(8, 124);
            this.checkBoxBlinking.Name = "checkBoxBlinking";
            this.checkBoxBlinking.Size = new System.Drawing.Size(63, 17);
            this.checkBoxBlinking.TabIndex = 15;
            this.checkBoxBlinking.Text = "Blinking";
            this.checkBoxBlinking.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(225, 13);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(28, 13);
            this.label2.TabIndex = 14;
            this.label2.Text = "Log:";
            // 
            // textBoxLog
            // 
            this.textBoxLog.Location = new System.Drawing.Point(225, 37);
            this.textBoxLog.Multiline = true;
            this.textBoxLog.Name = "textBoxLog";
            this.textBoxLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxLog.Size = new System.Drawing.Size(695, 292);
            this.textBoxLog.TabIndex = 13;
            // 
            // serialPortLine
            // 
            this.serialPortLine.BaudRate = 1200;
            this.serialPortLine.CRLFToAppend = "\\r\\n";
            this.serialPortLine.LineSeparators = new char[] {
        '\r',
        '\n'};
            this.serialPortLine.Location = new System.Drawing.Point(6, 6);
            this.serialPortLine.Name = "serialPortLine";
            this.serialPortLine.PortName = "COM10";
            this.serialPortLine.Size = new System.Drawing.Size(213, 112);
            this.serialPortLine.TabIndex = 12;
            this.serialPortLine.LineRead += new CSUtils.SerialPortLine.LineReadHandler(this.serialPortLine_LineRead);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.buttonParseServerMemoryLog);
            this.tabPage2.Controls.Add(this.textBoxServerMemoryLog);
            this.tabPage2.Controls.Add(this.buttonUpdateControls);
            this.tabPage2.Controls.Add(this.buttonUpdateMemory);
            this.tabPage2.Controls.Add(this.groupBox1);
            this.tabPage2.Controls.Add(this.memoryAreaControl8);
            this.tabPage2.Controls.Add(this.memoryAreaControl5);
            this.tabPage2.Controls.Add(this.memoryAreaControl6);
            this.tabPage2.Controls.Add(this.memoryAreaControl7);
            this.tabPage2.Controls.Add(this.memoryAreaControl2);
            this.tabPage2.Controls.Add(this.memoryAreaControl1);
            this.tabPage2.Controls.Add(this.memoryAreaControl4);
            this.tabPage2.Controls.Add(this.memoryAreaControl3);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(928, 510);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Debugging";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // buttonParseServerMemoryLog
            // 
            this.buttonParseServerMemoryLog.Location = new System.Drawing.Point(788, 174);
            this.buttonParseServerMemoryLog.Name = "buttonParseServerMemoryLog";
            this.buttonParseServerMemoryLog.Size = new System.Drawing.Size(122, 23);
            this.buttonParseServerMemoryLog.TabIndex = 17;
            this.buttonParseServerMemoryLog.Text = "Parse Memory Log";
            this.buttonParseServerMemoryLog.UseVisualStyleBackColor = true;
            this.buttonParseServerMemoryLog.Click += new System.EventHandler(this.buttonParseServerMemoryLog_Click);
            // 
            // textBoxServerMemoryLog
            // 
            this.textBoxServerMemoryLog.Location = new System.Drawing.Point(507, 206);
            this.textBoxServerMemoryLog.Multiline = true;
            this.textBoxServerMemoryLog.Name = "textBoxServerMemoryLog";
            this.textBoxServerMemoryLog.Size = new System.Drawing.Size(413, 107);
            this.textBoxServerMemoryLog.TabIndex = 16;
            // 
            // buttonUpdateControls
            // 
            this.buttonUpdateControls.Location = new System.Drawing.Point(788, 145);
            this.buttonUpdateControls.Name = "buttonUpdateControls";
            this.buttonUpdateControls.Size = new System.Drawing.Size(122, 23);
            this.buttonUpdateControls.TabIndex = 15;
            this.buttonUpdateControls.Text = "Update controls";
            this.buttonUpdateControls.UseVisualStyleBackColor = true;
            this.buttonUpdateControls.Click += new System.EventHandler(this.buttonUpdateControls_Click);
            // 
            // buttonUpdateMemory
            // 
            this.buttonUpdateMemory.Location = new System.Drawing.Point(788, 116);
            this.buttonUpdateMemory.Name = "buttonUpdateMemory";
            this.buttonUpdateMemory.Size = new System.Drawing.Size(122, 23);
            this.buttonUpdateMemory.TabIndex = 11;
            this.buttonUpdateMemory.Text = "Update memory";
            this.buttonUpdateMemory.UseVisualStyleBackColor = true;
            this.buttonUpdateMemory.Click += new System.EventHandler(this.buttonUpdateMemory_Click);
            // 
            // memoryAreaControl8
            // 
            this.memoryAreaControl8.Location = new System.Drawing.Point(599, 0);
            this.memoryAreaControl8.MemoryAddress = 27;
            this.memoryAreaControl8.MemoryData = "5A A5";
            this.memoryAreaControl8.MemoryLength = 2;
            this.memoryAreaControl8.Name = "memoryAreaControl8";
            this.memoryAreaControl8.Size = new System.Drawing.Size(158, 97);
            this.memoryAreaControl8.TabIndex = 8;
            // 
            // memoryAreaControl5
            // 
            this.memoryAreaControl5.Location = new System.Drawing.Point(0, 412);
            this.memoryAreaControl5.MemoryAddress = 47056;
            this.memoryAreaControl5.MemoryData = "FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 15 00 00 79 7F 42 14 0D FF 3C EE " +
                "00 00 85 00 05 39";
            this.memoryAreaControl5.MemoryLength = 33;
            this.memoryAreaControl5.Name = "memoryAreaControl5";
            this.memoryAreaControl5.Size = new System.Drawing.Size(590, 97);
            this.memoryAreaControl5.TabIndex = 8;
            // 
            // memoryAreaControl6
            // 
            this.memoryAreaControl6.Location = new System.Drawing.Point(0, 309);
            this.memoryAreaControl6.MemoryAddress = 46619;
            this.memoryAreaControl6.MemoryData = "01 00 96 00 32 00 32 00 00 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF " +
                "FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF";
            this.memoryAreaControl6.MemoryLength = 48;
            this.memoryAreaControl6.Name = "memoryAreaControl6";
            this.memoryAreaControl6.Size = new System.Drawing.Size(830, 97);
            this.memoryAreaControl6.TabIndex = 9;
            // 
            // memoryAreaControl7
            // 
            this.memoryAreaControl7.Location = new System.Drawing.Point(237, 103);
            this.memoryAreaControl7.MemoryAddress = 64;
            this.memoryAreaControl7.MemoryData = "41 F5 40 FF FF 00 00 00 00 00 00 00 00 00 01 2C";
            this.memoryAreaControl7.MemoryLength = 16;
            this.memoryAreaControl7.Name = "memoryAreaControl7";
            this.memoryAreaControl7.Size = new System.Drawing.Size(354, 97);
            this.memoryAreaControl7.TabIndex = 10;
            // 
            // memoryAreaControl2
            // 
            this.memoryAreaControl2.Location = new System.Drawing.Point(0, 0);
            this.memoryAreaControl2.MemoryAddress = 0;
            this.memoryAreaControl2.MemoryData = "00 00 FF F0 57 88 63 64 00 54 64 63 17 00 00 6E 48 2A 40 00 51 E7 B2 00 00 00 00 " +
                "05 5A 22 00 00";
            this.memoryAreaControl2.MemoryLength = 32;
            this.memoryAreaControl2.Name = "memoryAreaControl2";
            this.memoryAreaControl2.Size = new System.Drawing.Size(593, 97);
            this.memoryAreaControl2.TabIndex = 5;
            // 
            // memoryAreaControl1
            // 
            this.memoryAreaControl1.Location = new System.Drawing.Point(0, 206);
            this.memoryAreaControl1.MemoryAddress = 46592;
            this.memoryAreaControl1.MemoryData = "FF 00 55 68 00 00 02 00 00 32 00 00 42 08 80 64 01 4E AE 00 00 00 00 0E 00 00 01";
            this.memoryAreaControl1.MemoryLength = 27;
            this.memoryAreaControl1.Name = "memoryAreaControl1";
            this.memoryAreaControl1.Size = new System.Drawing.Size(501, 97);
            this.memoryAreaControl1.TabIndex = 2;
            // 
            // memoryAreaControl4
            // 
            this.memoryAreaControl4.Location = new System.Drawing.Point(763, 0);
            this.memoryAreaControl4.MemoryAddress = 47;
            this.memoryAreaControl4.MemoryData = "FF";
            this.memoryAreaControl4.MemoryLength = 1;
            this.memoryAreaControl4.Name = "memoryAreaControl4";
            this.memoryAreaControl4.Size = new System.Drawing.Size(139, 97);
            this.memoryAreaControl4.TabIndex = 7;
            // 
            // memoryAreaControl3
            // 
            this.memoryAreaControl3.Location = new System.Drawing.Point(0, 103);
            this.memoryAreaControl3.MemoryAddress = 54;
            this.memoryAreaControl3.MemoryData = "00 65 3C A0 10 C4 80 20 FF FD";
            this.memoryAreaControl3.MemoryLength = 10;
            this.memoryAreaControl3.Name = "memoryAreaControl3";
            this.memoryAreaControl3.Size = new System.Drawing.Size(231, 97);
            this.memoryAreaControl3.TabIndex = 6;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(936, 575);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "Lamp emulator";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.Timer timerStartup;
        private MemoryAreaControl memoryAreaControl1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxTResponseData;
        private MemoryAreaControl memoryAreaControl2;
        private MemoryAreaControl memoryAreaControl3;
        private MemoryAreaControl memoryAreaControl4;
        private MemoryAreaControl memoryAreaControl5;
        private MemoryAreaControl memoryAreaControl6;
        private MemoryAreaControl memoryAreaControl7;
        private System.Windows.Forms.Timer timerBlinking;
        private CSUtils.SerialPortLine serialPortLine;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private MemoryAreaControl memoryAreaControl8;
        private System.Windows.Forms.TextBox textBoxLog;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonUpdateControls;
        private System.Windows.Forms.Button buttonUpdateMemory;
        private System.Windows.Forms.CheckBox checkBoxBlinking;
        private System.Windows.Forms.Button buttonLogClear;
        private System.Windows.Forms.CheckBox checkBoxKeepTimeCurrent;
        private System.Windows.Forms.TextBox textBoxServerMemoryLog;
        private System.Windows.Forms.Button buttonParseServerMemoryLog;
    }
}

