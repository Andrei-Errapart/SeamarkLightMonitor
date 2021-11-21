namespace LampAdmin
{
    partial class FormConnection
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
            this.label22 = new System.Windows.Forms.Label();
            this.textBoxUserName = new System.Windows.Forms.TextBox();
            this.textBoxPassword = new System.Windows.Forms.TextBox();
            this.label21 = new System.Windows.Forms.Label();
            this.textBoxServer2 = new System.Windows.Forms.TextBox();
            this.textBoxServer1 = new System.Windows.Forms.TextBox();
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.radioButtonServer1 = new System.Windows.Forms.RadioButton();
            this.radioButtonServer2 = new System.Windows.Forms.RadioButton();
            this.SuspendLayout();
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(14, 15);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(58, 13);
            this.label22.TabIndex = 21;
            this.label22.Text = "Username:";
            // 
            // textBoxUserName
            // 
            this.textBoxUserName.Location = new System.Drawing.Point(91, 12);
            this.textBoxUserName.Name = "textBoxUserName";
            this.textBoxUserName.Size = new System.Drawing.Size(100, 20);
            this.textBoxUserName.TabIndex = 20;
            this.textBoxUserName.Text = "Peeter";
            // 
            // textBoxPassword
            // 
            this.textBoxPassword.Location = new System.Drawing.Point(91, 41);
            this.textBoxPassword.Name = "textBoxPassword";
            this.textBoxPassword.PasswordChar = '*';
            this.textBoxPassword.Size = new System.Drawing.Size(100, 20);
            this.textBoxPassword.TabIndex = 19;
            this.textBoxPassword.Text = "PoleParooli";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(14, 44);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(56, 13);
            this.label21.TabIndex = 18;
            this.label21.Text = "Password:";
            // 
            // textBoxServer2
            // 
            this.textBoxServer2.Location = new System.Drawing.Point(91, 106);
            this.textBoxServer2.Name = "textBoxServer2";
            this.textBoxServer2.Size = new System.Drawing.Size(130, 20);
            this.textBoxServer2.TabIndex = 15;
            this.textBoxServer2.Text = "194.204.26.104:8888";
            // 
            // textBoxServer1
            // 
            this.textBoxServer1.Location = new System.Drawing.Point(91, 77);
            this.textBoxServer1.Name = "textBoxServer1";
            this.textBoxServer1.Size = new System.Drawing.Size(130, 20);
            this.textBoxServer1.TabIndex = 13;
            this.textBoxServer1.Text = "194.204.26.104:8080";
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(17, 144);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 24;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(106, 144);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 25;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // radioButtonServer1
            // 
            this.radioButtonServer1.AutoSize = true;
            this.radioButtonServer1.Checked = true;
            this.radioButtonServer1.Location = new System.Drawing.Point(17, 78);
            this.radioButtonServer1.Name = "radioButtonServer1";
            this.radioButtonServer1.Size = new System.Drawing.Size(68, 17);
            this.radioButtonServer1.TabIndex = 26;
            this.radioButtonServer1.TabStop = true;
            this.radioButtonServer1.Text = "Server 1:";
            this.radioButtonServer1.UseVisualStyleBackColor = true;
            // 
            // radioButtonServer2
            // 
            this.radioButtonServer2.AutoSize = true;
            this.radioButtonServer2.Location = new System.Drawing.Point(17, 107);
            this.radioButtonServer2.Name = "radioButtonServer2";
            this.radioButtonServer2.Size = new System.Drawing.Size(68, 17);
            this.radioButtonServer2.TabIndex = 27;
            this.radioButtonServer2.Text = "Server 2:";
            this.radioButtonServer2.UseVisualStyleBackColor = true;
            // 
            // FormConnection
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonCancel;
            this.ClientSize = new System.Drawing.Size(232, 175);
            this.Controls.Add(this.radioButtonServer2);
            this.Controls.Add(this.radioButtonServer1);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOK);
            this.Controls.Add(this.label22);
            this.Controls.Add(this.textBoxUserName);
            this.Controls.Add(this.textBoxPassword);
            this.Controls.Add(this.label21);
            this.Controls.Add(this.textBoxServer2);
            this.Controls.Add(this.textBoxServer1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "FormConnection";
            this.Text = "Server connection";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.TextBox textBoxUserName;
        private System.Windows.Forms.TextBox textBoxPassword;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.TextBox textBoxServer2;
        private System.Windows.Forms.TextBox textBoxServer1;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.RadioButton radioButtonServer1;
        private System.Windows.Forms.RadioButton radioButtonServer2;
    }
}