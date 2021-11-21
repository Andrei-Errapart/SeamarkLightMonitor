using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace LampEmulator
{
    public partial class MemoryAreaControl : UserControl
    {
        public MemoryAreaControl()
        {
            InitializeComponent();
        }

        private int Address_ = 0;
        private int Length_ = 0;

        private void UpdateFrameText_()
        {
            groupBoxFrame.Text = String.Format("Address {0:X4}, {1} bytes.", Address_, Length_);
        }

        public int MemoryAddress
        {
            get { return Address_; }
            set
            {
                Address_ = value;
                UpdateFrameText_();
            }
        }

        public int MemoryLength
        {
            get { return Length_; }
            set
            {
                Length_ = value;
                UpdateFrameText_();

                // Regenerate indices
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < Length_; ++i)
                {
                    sb.AppendFormat("{0:X02}", i);
                    if (i + 1 < Length_)
                    {
                        sb.Append(' ');
                    }
                }
                textBoxIndices.Text = sb.ToString();
            }
        }

        public string MemoryData
        {
            get { return textBoxData.Text; }
            set { textBoxData.Text = value; }
        }
    }
}
