using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace XEthernetDemo
{
    public partial class StartMenu : Form
    {
        public StartMenu()
        {
            InitializeComponent();
            Init_Menu();
        }

        private void Init_Menu()
        {

            foreach (Control c in this.Controls)
            {
                if (c.Tag.ToString() == "1")
                {
                    c.Enabled = false;
                    c.Visible = false;
                }
                else if (c.Tag.ToString() == "0")
                {
                    c.Enabled = true;
                    c.Visible = true;
                }
            }
            //label1.Visible = false;
        }

        private void SettingButton_Click(object sender, EventArgs e)
        {
            /*
            DebugButton.Visible = false;
            DebugButton.Enabled = false;
            TestButton.Visible = false;
            TestButton.Enabled = false;
            SettingButton.Visible = false;
            SettingButton.Enabled = false;
            */
            foreach(Control c in this.Controls)
            {
                if (c.Tag.ToString() == "0")
                {
                    c.Enabled = false;
                    c.Visible = false;
                }
                else if (c.Tag.ToString() == "1")
                {
                    c.Enabled = true;
                    c.Visible = true;
                }
            }
        }

        private void ChangeSetiingButton_Click(object sender, EventArgs e)
        {
            MessageBox.Show("参数修改成功！");
        }

        private void ReturnButton_Click(object sender, EventArgs e)
        {
            foreach (Control c in this.Controls)
            {
                if (c.Tag.ToString() == "1")
                {
                    c.Enabled = false;
                    c.Visible = false;
                }
                else if (c.Tag.ToString() == "0")
                {
                    c.Enabled = true;
                    c.Visible = true;
                }
            }
        }

        private void DebugButton_Click(object sender, EventArgs e)
        {
            Form1 f1 = new Form1();
            f1.speed = Convert.ToSingle(Speed.Text);
            f1.num_of_mouth = Convert.ToInt32(MouthNum.Text);
            f1.length_belt = Convert.ToInt32(BeltLength.Text);
            f1.length_linearray = Convert.ToInt32(ArrayLength.Text);
            f1.SOD = Convert.ToInt32(SOD.Text);
            f1.SDD = Convert.ToInt32(SDD.Text);
        }
    }
}
