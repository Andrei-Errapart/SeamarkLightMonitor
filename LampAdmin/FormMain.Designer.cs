namespace LampAdmin
{
    partial class FormMain
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
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.connectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.textBoxFilterRegNr = new System.Windows.Forms.TextBox();
            this.textBoxNavCurrent = new System.Windows.Forms.TextBox();
            this.tableLayoutPanelLamp = new System.Windows.Forms.TableLayoutPanel();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.textBoxNewChargerCutOutVoltage = new System.Windows.Forms.TextBox();
            this.textBoxNewChargerCutInVoltage = new System.Windows.Forms.TextBox();
            this.textBoxNewPhotocellThreshold = new System.Windows.Forms.TextBox();
            this.textBoxNewBatteryHysteresis = new System.Windows.Forms.TextBox();
            this.textBoxNewBatteryVoltageMin = new System.Windows.Forms.TextBox();
            this.checkBoxNewBlinkingEnabled = new System.Windows.Forms.CheckBox();
            this.textBoxNewBlinkingPattern = new System.Windows.Forms.TextBox();
            this.textBoxNewLampVoltage = new System.Windows.Forms.TextBox();
            this.textBoxNewOwner = new System.Windows.Forms.TextBox();
            this.textBoxNewPhoneNo = new System.Windows.Forms.TextBox();
            this.checkBoxNewIsPublic = new System.Windows.Forms.CheckBox();
            this.textBoxOldChargerCutOutVoltage = new System.Windows.Forms.TextBox();
            this.textBoxOldChargerCutInVoltage = new System.Windows.Forms.TextBox();
            this.textBoxOldPhotocellThreshold = new System.Windows.Forms.TextBox();
            this.textBoxOldBatteryHysteresis = new System.Windows.Forms.TextBox();
            this.textBoxOldBatteryVoltageMin = new System.Windows.Forms.TextBox();
            this.checkBoxOldBlinkingEnabled = new System.Windows.Forms.CheckBox();
            this.textBoxOldBlinkingPattern = new System.Windows.Forms.TextBox();
            this.textBoxOldLampVoltage = new System.Windows.Forms.TextBox();
            this.textBoxOldOwner = new System.Windows.Forms.TextBox();
            this.textBoxOldSerialNo = new System.Windows.Forms.TextBox();
            this.textBoxOldPhoneNo = new System.Windows.Forms.TextBox();
            this.textBoxOldIMEI = new System.Windows.Forms.TextBox();
            this.checkBoxOldIsPublic = new System.Windows.Forms.CheckBox();
            this.textBoxOldID = new System.Windows.Forms.TextBox();
            this.label20 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxNewIMEI = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.checkBoxOldKeepOffline = new System.Windows.Forms.CheckBox();
            this.checkBoxNewKeepOffline = new System.Windows.Forms.CheckBox();
            this.radioButtonRegNr = new System.Windows.Forms.RadioButton();
            this.radioButtonNew = new System.Windows.Forms.RadioButton();
            this.radioButtonAll = new System.Windows.Forms.RadioButton();
            this.label4 = new System.Windows.Forms.Label();
            this.buttonNavLast = new System.Windows.Forms.Button();
            this.buttonNavNext = new System.Windows.Forms.Button();
            this.buttonNavPrevious = new System.Windows.Forms.Button();
            this.buttonNavFirst = new System.Windows.Forms.Button();
            this.simpleTcpClient = new CSUtils.SimpleTcpClient();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxLog = new System.Windows.Forms.TextBox();
            this.buttonSend = new System.Windows.Forms.Button();
            this.textBoxSend = new System.Windows.Forms.TextBox();
            this.buttonUploadNew = new System.Windows.Forms.Button();
            this.buttonShutdown = new System.Windows.Forms.Button();
            this.buttonStayOnline = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.buttonSendAllRefreshSensors = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonLight7Send = new System.Windows.Forms.Button();
            this.textBoxLight7Default = new System.Windows.Forms.TextBox();
            this.label22 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.textBoxLight7Current = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.buttonLight8Send = new System.Windows.Forms.Button();
            this.label24 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.textBoxLight8Default = new System.Windows.Forms.TextBox();
            this.textBoxLight8Current = new System.Windows.Forms.TextBox();
            this.menuStripMain.SuspendLayout();
            this.tableLayoutPanelLamp.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStripMain
            // 
            this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.Size = new System.Drawing.Size(896, 24);
            this.menuStripMain.TabIndex = 1;
            this.menuStripMain.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.connectToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // connectToolStripMenuItem
            // 
            this.connectToolStripMenuItem.Name = "connectToolStripMenuItem";
            this.connectToolStripMenuItem.Size = new System.Drawing.Size(125, 22);
            this.connectToolStripMenuItem.Text = "&Connect";
            this.connectToolStripMenuItem.Click += new System.EventHandler(this.connectToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(125, 22);
            this.exitToolStripMenuItem.Text = "&Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // textBoxFilterRegNr
            // 
            this.textBoxFilterRegNr.Location = new System.Drawing.Point(230, 27);
            this.textBoxFilterRegNr.Name = "textBoxFilterRegNr";
            this.textBoxFilterRegNr.Size = new System.Drawing.Size(100, 20);
            this.textBoxFilterRegNr.TabIndex = 21;
            // 
            // textBoxNavCurrent
            // 
            this.textBoxNavCurrent.Location = new System.Drawing.Point(130, 441);
            this.textBoxNavCurrent.Name = "textBoxNavCurrent";
            this.textBoxNavCurrent.ReadOnly = true;
            this.textBoxNavCurrent.Size = new System.Drawing.Size(58, 20);
            this.textBoxNavCurrent.TabIndex = 18;
            // 
            // tableLayoutPanelLamp
            // 
            this.tableLayoutPanelLamp.ColumnCount = 3;
            this.tableLayoutPanelLamp.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanelLamp.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanelLamp.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.tableLayoutPanelLamp.Controls.Add(this.label5, 0, 0);
            this.tableLayoutPanelLamp.Controls.Add(this.label6, 1, 0);
            this.tableLayoutPanelLamp.Controls.Add(this.label7, 2, 0);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxNewChargerCutOutVoltage, 2, 14);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxNewChargerCutInVoltage, 2, 13);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxNewPhotocellThreshold, 2, 12);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxNewBatteryHysteresis, 2, 11);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxNewBatteryVoltageMin, 2, 10);
            this.tableLayoutPanelLamp.Controls.Add(this.checkBoxNewBlinkingEnabled, 2, 9);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxNewBlinkingPattern, 2, 8);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxNewLampVoltage, 2, 7);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxNewOwner, 2, 6);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxNewPhoneNo, 2, 4);
            this.tableLayoutPanelLamp.Controls.Add(this.checkBoxNewIsPublic, 2, 2);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldChargerCutOutVoltage, 1, 14);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldChargerCutInVoltage, 1, 13);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldPhotocellThreshold, 1, 12);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldBatteryHysteresis, 1, 11);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldBatteryVoltageMin, 1, 10);
            this.tableLayoutPanelLamp.Controls.Add(this.checkBoxOldBlinkingEnabled, 1, 9);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldBlinkingPattern, 1, 8);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldLampVoltage, 1, 7);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldOwner, 1, 6);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldSerialNo, 1, 5);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldPhoneNo, 1, 4);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldIMEI, 1, 3);
            this.tableLayoutPanelLamp.Controls.Add(this.checkBoxOldIsPublic, 1, 2);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxOldID, 1, 1);
            this.tableLayoutPanelLamp.Controls.Add(this.label20, 0, 14);
            this.tableLayoutPanelLamp.Controls.Add(this.label19, 0, 13);
            this.tableLayoutPanelLamp.Controls.Add(this.label17, 0, 12);
            this.tableLayoutPanelLamp.Controls.Add(this.label16, 0, 11);
            this.tableLayoutPanelLamp.Controls.Add(this.label15, 0, 10);
            this.tableLayoutPanelLamp.Controls.Add(this.label14, 0, 9);
            this.tableLayoutPanelLamp.Controls.Add(this.label13, 0, 8);
            this.tableLayoutPanelLamp.Controls.Add(this.label12, 0, 7);
            this.tableLayoutPanelLamp.Controls.Add(this.label11, 0, 6);
            this.tableLayoutPanelLamp.Controls.Add(this.label18, 0, 5);
            this.tableLayoutPanelLamp.Controls.Add(this.label10, 0, 4);
            this.tableLayoutPanelLamp.Controls.Add(this.label8, 0, 3);
            this.tableLayoutPanelLamp.Controls.Add(this.label9, 0, 2);
            this.tableLayoutPanelLamp.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanelLamp.Controls.Add(this.textBoxNewIMEI, 2, 3);
            this.tableLayoutPanelLamp.Controls.Add(this.label3, 0, 15);
            this.tableLayoutPanelLamp.Controls.Add(this.checkBoxOldKeepOffline, 1, 15);
            this.tableLayoutPanelLamp.Controls.Add(this.checkBoxNewKeepOffline, 2, 15);
            this.tableLayoutPanelLamp.Location = new System.Drawing.Point(16, 51);
            this.tableLayoutPanelLamp.Name = "tableLayoutPanelLamp";
            this.tableLayoutPanelLamp.RowCount = 16;
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanelLamp.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanelLamp.Size = new System.Drawing.Size(594, 382);
            this.tableLayoutPanelLamp.TabIndex = 15;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 13);
            this.label5.TabIndex = 0;
            this.label5.Text = "Field";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(200, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(41, 13);
            this.label6.TabIndex = 1;
            this.label6.Text = "Current";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(398, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(29, 13);
            this.label7.TabIndex = 2;
            this.label7.Text = "New";
            // 
            // textBoxNewChargerCutOutVoltage
            // 
            this.textBoxNewChargerCutOutVoltage.Location = new System.Drawing.Point(398, 342);
            this.textBoxNewChargerCutOutVoltage.Name = "textBoxNewChargerCutOutVoltage";
            this.textBoxNewChargerCutOutVoltage.Size = new System.Drawing.Size(100, 20);
            this.textBoxNewChargerCutOutVoltage.TabIndex = 42;
            // 
            // textBoxNewChargerCutInVoltage
            // 
            this.textBoxNewChargerCutInVoltage.Location = new System.Drawing.Point(398, 316);
            this.textBoxNewChargerCutInVoltage.Name = "textBoxNewChargerCutInVoltage";
            this.textBoxNewChargerCutInVoltage.Size = new System.Drawing.Size(100, 20);
            this.textBoxNewChargerCutInVoltage.TabIndex = 36;
            // 
            // textBoxNewPhotocellThreshold
            // 
            this.textBoxNewPhotocellThreshold.Location = new System.Drawing.Point(398, 290);
            this.textBoxNewPhotocellThreshold.Name = "textBoxNewPhotocellThreshold";
            this.textBoxNewPhotocellThreshold.Size = new System.Drawing.Size(100, 20);
            this.textBoxNewPhotocellThreshold.TabIndex = 35;
            // 
            // textBoxNewBatteryHysteresis
            // 
            this.textBoxNewBatteryHysteresis.Location = new System.Drawing.Point(398, 264);
            this.textBoxNewBatteryHysteresis.Name = "textBoxNewBatteryHysteresis";
            this.textBoxNewBatteryHysteresis.Size = new System.Drawing.Size(100, 20);
            this.textBoxNewBatteryHysteresis.TabIndex = 34;
            // 
            // textBoxNewBatteryVoltageMin
            // 
            this.textBoxNewBatteryVoltageMin.Location = new System.Drawing.Point(398, 238);
            this.textBoxNewBatteryVoltageMin.Name = "textBoxNewBatteryVoltageMin";
            this.textBoxNewBatteryVoltageMin.Size = new System.Drawing.Size(100, 20);
            this.textBoxNewBatteryVoltageMin.TabIndex = 33;
            // 
            // checkBoxNewBlinkingEnabled
            // 
            this.checkBoxNewBlinkingEnabled.AutoSize = true;
            this.checkBoxNewBlinkingEnabled.Location = new System.Drawing.Point(398, 218);
            this.checkBoxNewBlinkingEnabled.Name = "checkBoxNewBlinkingEnabled";
            this.checkBoxNewBlinkingEnabled.Size = new System.Drawing.Size(15, 14);
            this.checkBoxNewBlinkingEnabled.TabIndex = 25;
            this.checkBoxNewBlinkingEnabled.UseVisualStyleBackColor = true;
            // 
            // textBoxNewBlinkingPattern
            // 
            this.textBoxNewBlinkingPattern.Location = new System.Drawing.Point(398, 192);
            this.textBoxNewBlinkingPattern.Name = "textBoxNewBlinkingPattern";
            this.textBoxNewBlinkingPattern.Size = new System.Drawing.Size(193, 20);
            this.textBoxNewBlinkingPattern.TabIndex = 27;
            // 
            // textBoxNewLampVoltage
            // 
            this.textBoxNewLampVoltage.Location = new System.Drawing.Point(398, 166);
            this.textBoxNewLampVoltage.Name = "textBoxNewLampVoltage";
            this.textBoxNewLampVoltage.Size = new System.Drawing.Size(100, 20);
            this.textBoxNewLampVoltage.TabIndex = 26;
            // 
            // textBoxNewOwner
            // 
            this.textBoxNewOwner.Location = new System.Drawing.Point(398, 140);
            this.textBoxNewOwner.Name = "textBoxNewOwner";
            this.textBoxNewOwner.Size = new System.Drawing.Size(100, 20);
            this.textBoxNewOwner.TabIndex = 21;
            // 
            // textBoxNewPhoneNo
            // 
            this.textBoxNewPhoneNo.Location = new System.Drawing.Point(398, 88);
            this.textBoxNewPhoneNo.Name = "textBoxNewPhoneNo";
            this.textBoxNewPhoneNo.Size = new System.Drawing.Size(100, 20);
            this.textBoxNewPhoneNo.TabIndex = 19;
            // 
            // checkBoxNewIsPublic
            // 
            this.checkBoxNewIsPublic.AutoSize = true;
            this.checkBoxNewIsPublic.Location = new System.Drawing.Point(398, 42);
            this.checkBoxNewIsPublic.Name = "checkBoxNewIsPublic";
            this.checkBoxNewIsPublic.Size = new System.Drawing.Size(15, 14);
            this.checkBoxNewIsPublic.TabIndex = 7;
            this.checkBoxNewIsPublic.UseVisualStyleBackColor = true;
            // 
            // textBoxOldChargerCutOutVoltage
            // 
            this.textBoxOldChargerCutOutVoltage.Location = new System.Drawing.Point(200, 342);
            this.textBoxOldChargerCutOutVoltage.Name = "textBoxOldChargerCutOutVoltage";
            this.textBoxOldChargerCutOutVoltage.ReadOnly = true;
            this.textBoxOldChargerCutOutVoltage.Size = new System.Drawing.Size(100, 20);
            this.textBoxOldChargerCutOutVoltage.TabIndex = 41;
            // 
            // textBoxOldChargerCutInVoltage
            // 
            this.textBoxOldChargerCutInVoltage.Location = new System.Drawing.Point(200, 316);
            this.textBoxOldChargerCutInVoltage.Name = "textBoxOldChargerCutInVoltage";
            this.textBoxOldChargerCutInVoltage.ReadOnly = true;
            this.textBoxOldChargerCutInVoltage.Size = new System.Drawing.Size(100, 20);
            this.textBoxOldChargerCutInVoltage.TabIndex = 40;
            // 
            // textBoxOldPhotocellThreshold
            // 
            this.textBoxOldPhotocellThreshold.Location = new System.Drawing.Point(200, 290);
            this.textBoxOldPhotocellThreshold.Name = "textBoxOldPhotocellThreshold";
            this.textBoxOldPhotocellThreshold.ReadOnly = true;
            this.textBoxOldPhotocellThreshold.Size = new System.Drawing.Size(100, 20);
            this.textBoxOldPhotocellThreshold.TabIndex = 39;
            // 
            // textBoxOldBatteryHysteresis
            // 
            this.textBoxOldBatteryHysteresis.Location = new System.Drawing.Point(200, 264);
            this.textBoxOldBatteryHysteresis.Name = "textBoxOldBatteryHysteresis";
            this.textBoxOldBatteryHysteresis.ReadOnly = true;
            this.textBoxOldBatteryHysteresis.Size = new System.Drawing.Size(100, 20);
            this.textBoxOldBatteryHysteresis.TabIndex = 38;
            // 
            // textBoxOldBatteryVoltageMin
            // 
            this.textBoxOldBatteryVoltageMin.Location = new System.Drawing.Point(200, 238);
            this.textBoxOldBatteryVoltageMin.Name = "textBoxOldBatteryVoltageMin";
            this.textBoxOldBatteryVoltageMin.ReadOnly = true;
            this.textBoxOldBatteryVoltageMin.Size = new System.Drawing.Size(100, 20);
            this.textBoxOldBatteryVoltageMin.TabIndex = 37;
            // 
            // checkBoxOldBlinkingEnabled
            // 
            this.checkBoxOldBlinkingEnabled.AutoSize = true;
            this.checkBoxOldBlinkingEnabled.Enabled = false;
            this.checkBoxOldBlinkingEnabled.Location = new System.Drawing.Point(200, 218);
            this.checkBoxOldBlinkingEnabled.Name = "checkBoxOldBlinkingEnabled";
            this.checkBoxOldBlinkingEnabled.Size = new System.Drawing.Size(15, 14);
            this.checkBoxOldBlinkingEnabled.TabIndex = 24;
            this.checkBoxOldBlinkingEnabled.UseVisualStyleBackColor = true;
            // 
            // textBoxOldBlinkingPattern
            // 
            this.textBoxOldBlinkingPattern.Location = new System.Drawing.Point(200, 192);
            this.textBoxOldBlinkingPattern.Name = "textBoxOldBlinkingPattern";
            this.textBoxOldBlinkingPattern.ReadOnly = true;
            this.textBoxOldBlinkingPattern.Size = new System.Drawing.Size(192, 20);
            this.textBoxOldBlinkingPattern.TabIndex = 32;
            // 
            // textBoxOldLampVoltage
            // 
            this.textBoxOldLampVoltage.Location = new System.Drawing.Point(200, 166);
            this.textBoxOldLampVoltage.Name = "textBoxOldLampVoltage";
            this.textBoxOldLampVoltage.ReadOnly = true;
            this.textBoxOldLampVoltage.Size = new System.Drawing.Size(100, 20);
            this.textBoxOldLampVoltage.TabIndex = 31;
            // 
            // textBoxOldOwner
            // 
            this.textBoxOldOwner.Location = new System.Drawing.Point(200, 140);
            this.textBoxOldOwner.Name = "textBoxOldOwner";
            this.textBoxOldOwner.ReadOnly = true;
            this.textBoxOldOwner.Size = new System.Drawing.Size(100, 20);
            this.textBoxOldOwner.TabIndex = 30;
            // 
            // textBoxOldSerialNo
            // 
            this.textBoxOldSerialNo.Location = new System.Drawing.Point(200, 114);
            this.textBoxOldSerialNo.Name = "textBoxOldSerialNo";
            this.textBoxOldSerialNo.ReadOnly = true;
            this.textBoxOldSerialNo.Size = new System.Drawing.Size(100, 20);
            this.textBoxOldSerialNo.TabIndex = 29;
            // 
            // textBoxOldPhoneNo
            // 
            this.textBoxOldPhoneNo.Location = new System.Drawing.Point(200, 88);
            this.textBoxOldPhoneNo.Name = "textBoxOldPhoneNo";
            this.textBoxOldPhoneNo.ReadOnly = true;
            this.textBoxOldPhoneNo.Size = new System.Drawing.Size(100, 20);
            this.textBoxOldPhoneNo.TabIndex = 28;
            // 
            // textBoxOldIMEI
            // 
            this.textBoxOldIMEI.Location = new System.Drawing.Point(200, 62);
            this.textBoxOldIMEI.Name = "textBoxOldIMEI";
            this.textBoxOldIMEI.ReadOnly = true;
            this.textBoxOldIMEI.Size = new System.Drawing.Size(192, 20);
            this.textBoxOldIMEI.TabIndex = 22;
            // 
            // checkBoxOldIsPublic
            // 
            this.checkBoxOldIsPublic.AutoSize = true;
            this.checkBoxOldIsPublic.Enabled = false;
            this.checkBoxOldIsPublic.Location = new System.Drawing.Point(200, 42);
            this.checkBoxOldIsPublic.Name = "checkBoxOldIsPublic";
            this.checkBoxOldIsPublic.Size = new System.Drawing.Size(15, 14);
            this.checkBoxOldIsPublic.TabIndex = 23;
            this.checkBoxOldIsPublic.UseVisualStyleBackColor = true;
            // 
            // textBoxOldID
            // 
            this.textBoxOldID.Location = new System.Drawing.Point(200, 16);
            this.textBoxOldID.Name = "textBoxOldID";
            this.textBoxOldID.ReadOnly = true;
            this.textBoxOldID.Size = new System.Drawing.Size(100, 20);
            this.textBoxOldID.TabIndex = 43;
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(3, 339);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(121, 13);
            this.label20.TabIndex = 18;
            this.label20.Text = "Charger cut-out voltage:";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(3, 313);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(114, 13);
            this.label19.TabIndex = 17;
            this.label19.Text = "Charger cut-in voltage:";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(3, 287);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(100, 13);
            this.label17.TabIndex = 15;
            this.label17.Text = "Photocell threshold:";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(3, 261);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(92, 13);
            this.label16.TabIndex = 14;
            this.label16.Text = "Battery hysteresis:";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(3, 235);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(100, 13);
            this.label15.TabIndex = 13;
            this.label15.Text = "Battery voltage min:";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(3, 215);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(88, 13);
            this.label14.TabIndex = 12;
            this.label14.Text = "Blinking enabled:";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(3, 189);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(83, 13);
            this.label13.TabIndex = 11;
            this.label13.Text = "Blinking pattern:";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(3, 163);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(74, 13);
            this.label12.TabIndex = 10;
            this.label12.Text = "Lamp voltage:";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(3, 137);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(41, 13);
            this.label11.TabIndex = 9;
            this.label11.Text = "Owner:";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(3, 111);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(51, 13);
            this.label18.TabIndex = 16;
            this.label18.Text = "Serial no:";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(3, 85);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(56, 13);
            this.label10.TabIndex = 8;
            this.label10.Text = "Phone no:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(3, 59);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(32, 13);
            this.label8.TabIndex = 3;
            this.label8.Text = "IMEI:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(3, 39);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(49, 13);
            this.label9.TabIndex = 6;
            this.label9.Text = "Is public:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 13);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(21, 13);
            this.label2.TabIndex = 44;
            this.label2.Text = "ID:";
            // 
            // textBoxNewIMEI
            // 
            this.textBoxNewIMEI.Location = new System.Drawing.Point(398, 62);
            this.textBoxNewIMEI.Name = "textBoxNewIMEI";
            this.textBoxNewIMEI.Size = new System.Drawing.Size(193, 20);
            this.textBoxNewIMEI.TabIndex = 45;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 359);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(62, 13);
            this.label3.TabIndex = 46;
            this.label3.Text = "KeepOffline";
            // 
            // checkBoxOldKeepOffline
            // 
            this.checkBoxOldKeepOffline.AutoSize = true;
            this.checkBoxOldKeepOffline.Location = new System.Drawing.Point(200, 362);
            this.checkBoxOldKeepOffline.Name = "checkBoxOldKeepOffline";
            this.checkBoxOldKeepOffline.Size = new System.Drawing.Size(15, 14);
            this.checkBoxOldKeepOffline.TabIndex = 47;
            this.checkBoxOldKeepOffline.UseVisualStyleBackColor = true;
            // 
            // checkBoxNewKeepOffline
            // 
            this.checkBoxNewKeepOffline.AutoSize = true;
            this.checkBoxNewKeepOffline.Location = new System.Drawing.Point(398, 362);
            this.checkBoxNewKeepOffline.Name = "checkBoxNewKeepOffline";
            this.checkBoxNewKeepOffline.Size = new System.Drawing.Size(15, 14);
            this.checkBoxNewKeepOffline.TabIndex = 48;
            this.checkBoxNewKeepOffline.UseVisualStyleBackColor = true;
            // 
            // radioButtonRegNr
            // 
            this.radioButtonRegNr.AutoSize = true;
            this.radioButtonRegNr.Location = new System.Drawing.Point(161, 28);
            this.radioButtonRegNr.Name = "radioButtonRegNr";
            this.radioButtonRegNr.Size = new System.Drawing.Size(66, 17);
            this.radioButtonRegNr.TabIndex = 14;
            this.radioButtonRegNr.Text = "Reg. nr.:";
            this.radioButtonRegNr.UseVisualStyleBackColor = true;
            // 
            // radioButtonNew
            // 
            this.radioButtonNew.AutoSize = true;
            this.radioButtonNew.Location = new System.Drawing.Point(108, 28);
            this.radioButtonNew.Name = "radioButtonNew";
            this.radioButtonNew.Size = new System.Drawing.Size(47, 17);
            this.radioButtonNew.TabIndex = 13;
            this.radioButtonNew.Text = "New";
            this.radioButtonNew.UseVisualStyleBackColor = true;
            // 
            // radioButtonAll
            // 
            this.radioButtonAll.AutoSize = true;
            this.radioButtonAll.Checked = true;
            this.radioButtonAll.Location = new System.Drawing.Point(66, 28);
            this.radioButtonAll.Name = "radioButtonAll";
            this.radioButtonAll.Size = new System.Drawing.Size(36, 17);
            this.radioButtonAll.TabIndex = 12;
            this.radioButtonAll.TabStop = true;
            this.radioButtonAll.Text = "All";
            this.radioButtonAll.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(13, 30);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(32, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "Filter:";
            // 
            // buttonNavLast
            // 
            this.buttonNavLast.Image = global::LampAdmin.Properties.Resources.navigation_last;
            this.buttonNavLast.Location = new System.Drawing.Point(248, 439);
            this.buttonNavLast.Name = "buttonNavLast";
            this.buttonNavLast.Size = new System.Drawing.Size(48, 23);
            this.buttonNavLast.TabIndex = 20;
            this.buttonNavLast.UseVisualStyleBackColor = true;
            this.buttonNavLast.Click += new System.EventHandler(this.buttonNavLast_Click);
            // 
            // buttonNavNext
            // 
            this.buttonNavNext.Image = global::LampAdmin.Properties.Resources.navigation_next;
            this.buttonNavNext.Location = new System.Drawing.Point(194, 439);
            this.buttonNavNext.Name = "buttonNavNext";
            this.buttonNavNext.Size = new System.Drawing.Size(48, 23);
            this.buttonNavNext.TabIndex = 19;
            this.buttonNavNext.UseVisualStyleBackColor = true;
            this.buttonNavNext.Click += new System.EventHandler(this.buttonNavNext_Click);
            // 
            // buttonNavPrevious
            // 
            this.buttonNavPrevious.Image = global::LampAdmin.Properties.Resources.navigation_previous;
            this.buttonNavPrevious.Location = new System.Drawing.Point(76, 439);
            this.buttonNavPrevious.Name = "buttonNavPrevious";
            this.buttonNavPrevious.Size = new System.Drawing.Size(48, 23);
            this.buttonNavPrevious.TabIndex = 17;
            this.buttonNavPrevious.UseVisualStyleBackColor = true;
            this.buttonNavPrevious.Click += new System.EventHandler(this.buttonNavPrevious_Click);
            // 
            // buttonNavFirst
            // 
            this.buttonNavFirst.Image = global::LampAdmin.Properties.Resources.navigation_first;
            this.buttonNavFirst.Location = new System.Drawing.Point(22, 439);
            this.buttonNavFirst.Name = "buttonNavFirst";
            this.buttonNavFirst.Size = new System.Drawing.Size(48, 23);
            this.buttonNavFirst.TabIndex = 16;
            this.buttonNavFirst.UseVisualStyleBackColor = true;
            this.buttonNavFirst.Click += new System.EventHandler(this.buttonNavFirst_Click);
            // 
            // simpleTcpClient
            // 
            this.simpleTcpClient.AutoConnect = true;
            this.simpleTcpClient.DestinationHost = "";
            this.simpleTcpClient.DestinationPort = 0;
            this.simpleTcpClient.DataRead += new CSUtils.SimpleTcpClient.DataReadHandler(this.simpleTcpClient_DataRead);
            this.simpleTcpClient.ExceptionHandled += new CSUtils.SimpleTcpClient.ExceptionHandledHandler(this.simpleTcpClient_ExceptionHandled);
            this.simpleTcpClient.StatusChanged += new CSUtils.SimpleTcpClient.StatusChangedHandler(this.simpleTcpClient_StatusChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(624, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(28, 13);
            this.label1.TabIndex = 22;
            this.label1.Text = "Log:";
            // 
            // textBoxLog
            // 
            this.textBoxLog.Location = new System.Drawing.Point(616, 48);
            this.textBoxLog.Multiline = true;
            this.textBoxLog.Name = "textBoxLog";
            this.textBoxLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxLog.Size = new System.Drawing.Size(268, 347);
            this.textBoxLog.TabIndex = 23;
            // 
            // buttonSend
            // 
            this.buttonSend.Location = new System.Drawing.Point(535, 439);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(75, 23);
            this.buttonSend.TabIndex = 24;
            this.buttonSend.Text = "Send";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // textBoxSend
            // 
            this.textBoxSend.Location = new System.Drawing.Point(616, 432);
            this.textBoxSend.Name = "textBoxSend";
            this.textBoxSend.Size = new System.Drawing.Size(268, 20);
            this.textBoxSend.TabIndex = 25;
            // 
            // buttonUploadNew
            // 
            this.buttonUploadNew.Location = new System.Drawing.Point(414, 439);
            this.buttonUploadNew.Name = "buttonUploadNew";
            this.buttonUploadNew.Size = new System.Drawing.Size(75, 23);
            this.buttonUploadNew.TabIndex = 26;
            this.buttonUploadNew.Text = "Upload New";
            this.buttonUploadNew.UseVisualStyleBackColor = true;
            this.buttonUploadNew.Click += new System.EventHandler(this.buttonUploadNew_Click);
            // 
            // buttonShutdown
            // 
            this.buttonShutdown.Location = new System.Drawing.Point(616, 488);
            this.buttonShutdown.Name = "buttonShutdown";
            this.buttonShutdown.Size = new System.Drawing.Size(75, 23);
            this.buttonShutdown.TabIndex = 27;
            this.buttonShutdown.Text = "Shutdown";
            this.buttonShutdown.UseVisualStyleBackColor = true;
            this.buttonShutdown.Click += new System.EventHandler(this.buttonShutdown_Click);
            // 
            // buttonStayOnline
            // 
            this.buttonStayOnline.Location = new System.Drawing.Point(616, 459);
            this.buttonStayOnline.Name = "buttonStayOnline";
            this.buttonStayOnline.Size = new System.Drawing.Size(107, 23);
            this.buttonStayOnline.TabIndex = 28;
            this.buttonStayOnline.Text = "Set \"StayOnline\"";
            this.buttonStayOnline.UseVisualStyleBackColor = true;
            this.buttonStayOnline.Click += new System.EventHandler(this.buttonStayOnline_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(729, 458);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(111, 23);
            this.button1.TabIndex = 29;
            this.button1.Text = "Clear \"StayOnline\"";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // buttonSendAllRefreshSensors
            // 
            this.buttonSendAllRefreshSensors.Location = new System.Drawing.Point(729, 488);
            this.buttonSendAllRefreshSensors.Name = "buttonSendAllRefreshSensors";
            this.buttonSendAllRefreshSensors.Size = new System.Drawing.Size(111, 23);
            this.buttonSendAllRefreshSensors.TabIndex = 30;
            this.buttonSendAllRefreshSensors.Text = "Refresh All Sensors";
            this.buttonSendAllRefreshSensors.UseVisualStyleBackColor = true;
            this.buttonSendAllRefreshSensors.Click += new System.EventHandler(this.buttonSendAllRefreshSensors_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.buttonLight7Send);
            this.groupBox1.Controls.Add(this.textBoxLight7Default);
            this.groupBox1.Controls.Add(this.label22);
            this.groupBox1.Controls.Add(this.label21);
            this.groupBox1.Controls.Add(this.textBoxLight7Current);
            this.groupBox1.Location = new System.Drawing.Point(16, 468);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(194, 90);
            this.groupBox1.TabIndex = 31;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Light 7 intensity (0..255)";
            // 
            // buttonLight7Send
            // 
            this.buttonLight7Send.Location = new System.Drawing.Point(109, 15);
            this.buttonLight7Send.Name = "buttonLight7Send";
            this.buttonLight7Send.Size = new System.Drawing.Size(75, 23);
            this.buttonLight7Send.TabIndex = 4;
            this.buttonLight7Send.Text = "Send";
            this.buttonLight7Send.UseVisualStyleBackColor = true;
            this.buttonLight7Send.Click += new System.EventHandler(this.buttonLight7Send_Click);
            // 
            // textBoxLight7Default
            // 
            this.textBoxLight7Default.Location = new System.Drawing.Point(56, 43);
            this.textBoxLight7Default.Name = "textBoxLight7Default";
            this.textBoxLight7Default.Size = new System.Drawing.Size(47, 20);
            this.textBoxLight7Default.TabIndex = 3;
            this.textBoxLight7Default.Text = "25";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(6, 46);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(44, 13);
            this.label22.TabIndex = 2;
            this.label22.Text = "Default:";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(6, 20);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(44, 13);
            this.label21.TabIndex = 1;
            this.label21.Text = "Current:";
            // 
            // textBoxLight7Current
            // 
            this.textBoxLight7Current.Location = new System.Drawing.Point(56, 17);
            this.textBoxLight7Current.Name = "textBoxLight7Current";
            this.textBoxLight7Current.Size = new System.Drawing.Size(47, 20);
            this.textBoxLight7Current.TabIndex = 0;
            this.textBoxLight7Current.Text = "25";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.buttonLight8Send);
            this.groupBox2.Controls.Add(this.label24);
            this.groupBox2.Controls.Add(this.label23);
            this.groupBox2.Controls.Add(this.textBoxLight8Default);
            this.groupBox2.Controls.Add(this.textBoxLight8Current);
            this.groupBox2.Location = new System.Drawing.Point(216, 468);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(192, 90);
            this.groupBox2.TabIndex = 32;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Light 8 intensity (0..255)";
            // 
            // buttonLight8Send
            // 
            this.buttonLight8Send.Location = new System.Drawing.Point(109, 15);
            this.buttonLight8Send.Name = "buttonLight8Send";
            this.buttonLight8Send.Size = new System.Drawing.Size(75, 23);
            this.buttonLight8Send.TabIndex = 4;
            this.buttonLight8Send.Text = "Send";
            this.buttonLight8Send.UseVisualStyleBackColor = true;
            this.buttonLight8Send.Click += new System.EventHandler(this.buttonLight8Send_Click);
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(6, 46);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(44, 13);
            this.label24.TabIndex = 3;
            this.label24.Text = "Default:";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(6, 20);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(44, 13);
            this.label23.TabIndex = 2;
            this.label23.Text = "Current:";
            // 
            // textBoxLight8Default
            // 
            this.textBoxLight8Default.Location = new System.Drawing.Point(56, 43);
            this.textBoxLight8Default.Name = "textBoxLight8Default";
            this.textBoxLight8Default.Size = new System.Drawing.Size(47, 20);
            this.textBoxLight8Default.TabIndex = 1;
            this.textBoxLight8Default.Text = "30";
            // 
            // textBoxLight8Current
            // 
            this.textBoxLight8Current.Location = new System.Drawing.Point(56, 17);
            this.textBoxLight8Current.Name = "textBoxLight8Current";
            this.textBoxLight8Current.Size = new System.Drawing.Size(47, 20);
            this.textBoxLight8Current.TabIndex = 0;
            this.textBoxLight8Current.Text = "30";
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(896, 570);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.buttonSendAllRefreshSensors);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.buttonStayOnline);
            this.Controls.Add(this.buttonShutdown);
            this.Controls.Add(this.buttonUploadNew);
            this.Controls.Add(this.textBoxSend);
            this.Controls.Add(this.buttonSend);
            this.Controls.Add(this.textBoxLog);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBoxFilterRegNr);
            this.Controls.Add(this.buttonNavLast);
            this.Controls.Add(this.buttonNavNext);
            this.Controls.Add(this.textBoxNavCurrent);
            this.Controls.Add(this.buttonNavPrevious);
            this.Controls.Add(this.buttonNavFirst);
            this.Controls.Add(this.tableLayoutPanelLamp);
            this.Controls.Add(this.radioButtonRegNr);
            this.Controls.Add(this.radioButtonNew);
            this.Controls.Add(this.radioButtonAll);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.menuStripMain);
            this.MainMenuStrip = this.menuStripMain;
            this.Name = "FormMain";
            this.Text = "Lamp Administrator";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.menuStripMain.ResumeLayout(false);
            this.menuStripMain.PerformLayout();
            this.tableLayoutPanelLamp.ResumeLayout(false);
            this.tableLayoutPanelLamp.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStripMain;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.TextBox textBoxFilterRegNr;
        private System.Windows.Forms.Button buttonNavLast;
        private System.Windows.Forms.Button buttonNavNext;
        private System.Windows.Forms.TextBox textBoxNavCurrent;
        private System.Windows.Forms.Button buttonNavPrevious;
        private System.Windows.Forms.Button buttonNavFirst;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelLamp;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.CheckBox checkBoxNewIsPublic;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.TextBox textBoxNewPhoneNo;
        private System.Windows.Forms.TextBox textBoxNewOwner;
        private System.Windows.Forms.TextBox textBoxOldIMEI;
        private System.Windows.Forms.CheckBox checkBoxOldIsPublic;
        private System.Windows.Forms.CheckBox checkBoxOldBlinkingEnabled;
        private System.Windows.Forms.CheckBox checkBoxNewBlinkingEnabled;
        private System.Windows.Forms.TextBox textBoxNewLampVoltage;
        private System.Windows.Forms.TextBox textBoxNewBlinkingPattern;
        private System.Windows.Forms.TextBox textBoxOldPhoneNo;
        private System.Windows.Forms.TextBox textBoxOldSerialNo;
        private System.Windows.Forms.TextBox textBoxOldOwner;
        private System.Windows.Forms.TextBox textBoxOldLampVoltage;
        private System.Windows.Forms.TextBox textBoxOldBlinkingPattern;
        private System.Windows.Forms.TextBox textBoxNewBatteryVoltageMin;
        private System.Windows.Forms.TextBox textBoxNewBatteryHysteresis;
        private System.Windows.Forms.TextBox textBoxNewPhotocellThreshold;
        private System.Windows.Forms.TextBox textBoxNewChargerCutInVoltage;
        private System.Windows.Forms.TextBox textBoxOldBatteryVoltageMin;
        private System.Windows.Forms.TextBox textBoxOldBatteryHysteresis;
        private System.Windows.Forms.TextBox textBoxOldPhotocellThreshold;
        private System.Windows.Forms.TextBox textBoxOldChargerCutInVoltage;
        private System.Windows.Forms.TextBox textBoxOldChargerCutOutVoltage;
        private System.Windows.Forms.TextBox textBoxNewChargerCutOutVoltage;
        private System.Windows.Forms.RadioButton radioButtonRegNr;
        private System.Windows.Forms.RadioButton radioButtonNew;
        private System.Windows.Forms.RadioButton radioButtonAll;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ToolStripMenuItem connectToolStripMenuItem;
        private CSUtils.SimpleTcpClient simpleTcpClient;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxLog;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.TextBox textBoxSend;
        private System.Windows.Forms.Button buttonUploadNew;
        private System.Windows.Forms.TextBox textBoxOldID;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxNewIMEI;
        private System.Windows.Forms.Button buttonShutdown;
        private System.Windows.Forms.Button buttonStayOnline;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.CheckBox checkBoxOldKeepOffline;
        private System.Windows.Forms.CheckBox checkBoxNewKeepOffline;
        private System.Windows.Forms.Button buttonSendAllRefreshSensors;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button buttonLight7Send;
        private System.Windows.Forms.TextBox textBoxLight7Default;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.TextBox textBoxLight7Current;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button buttonLight8Send;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.TextBox textBoxLight8Default;
        private System.Windows.Forms.TextBox textBoxLight8Current;
    }
}

