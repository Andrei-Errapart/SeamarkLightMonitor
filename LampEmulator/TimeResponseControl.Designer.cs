namespace LampEmulator
{
    partial class TimeResponseControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBoxFrame = new System.Windows.Forms.GroupBox();
            this.SuspendLayout();
            // 
            // groupBoxFrame
            // 
            this.groupBoxFrame.Location = new System.Drawing.Point(3, 3);
            this.groupBoxFrame.Name = "groupBoxFrame";
            this.groupBoxFrame.Size = new System.Drawing.Size(349, 172);
            this.groupBoxFrame.TabIndex = 0;
            this.groupBoxFrame.TabStop = false;
            this.groupBoxFrame.Text = "Time, etc.";
            // 
            // TimeResponseControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBoxFrame);
            this.Name = "TimeResponseControl";
            this.Size = new System.Drawing.Size(355, 178);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxFrame;
    }
}
