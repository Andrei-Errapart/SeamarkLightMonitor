using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace LampAdmin
{
    public partial class FormConnection : Form
    {
        public class Settings
        {
            public string ServerName;
            public int ServerPort;
            public string Username;
            public string Password;
        }

        private Settings settings = null;

        public FormConnection()
        {
            InitializeComponent();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            settings = null;
            try
            {
                Settings r = new Settings();
                // Server info.
                {
                    string server = radioButtonServer1.Checked ? textBoxServer1.Text : textBoxServer2.Text;
                    string[] server_info = server.Split(new char[] { ':' });
                    r.ServerName = server_info[0];
                    r.ServerPort = int.Parse(server_info[1]);
                }
                r.Username = textBoxUserName.Text;
                r.Password = textBoxPassword.Text;

                settings = r;

                Close();
            }
            catch (Exception)
            {
                MessageBox.Show("Check the fields.", "Error");
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            settings = null;
            Close();
        }

        public Settings DoShowDialog(   IWin32Window    parent)
        {
            settings = null;
            ShowDialog(parent);
            return settings;
        }
    }
}