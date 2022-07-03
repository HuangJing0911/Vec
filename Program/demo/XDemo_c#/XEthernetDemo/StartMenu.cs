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
        bool setting_change = false;
        //new Sunisoft.IrisSkin.SkinEngine().SkinFile = "Skins/MacOS.ssk";
        public StartMenu()
        {
            InitializeComponent();
            Init_Menu();
        }

        private void Init_Menu()
        {

            foreach (Control c in this.Controls)
            {
                if (c.Tag.ToString() == "1" || c.Tag.ToString() == "2")
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
            foreach(Control c in this.Controls)
            {
                if (c.Tag.ToString() == "0")
                {
                    c.Enabled = false;
                    c.Visible = false;
                }
                else if (c.Tag.ToString() == "1")
                {
                    c.Enabled = false;
                    c.Visible = true;
                }
                else if (c.Tag.ToString() == "2")
                {
                    c.Enabled = true;
                    c.Visible = true;
                }
            }
        }

        private void ChangeSetiingButton_Click(object sender, EventArgs e)
        {
            if (!setting_change)
            {
                setting_change = true;
                foreach (Control c in this.Controls)
                {
                    if (c.Tag.ToString() == "1")
                    {
                        c.Enabled = true;
                    }
                    else if (c.Name.ToString() == "ReturnButton")
                        c.Enabled = false;
                }
            }
            else
            {
                setting_change = false;
                foreach (Control c in this.Controls)
                {
                    if (c.Tag.ToString() == "1")
                    {
                        c.Enabled = false;
                    }
                    else if (c.Name.ToString() == "ReturnButton")
                        c.Enabled = true;
                }
                MessageBox.Show("参数修改成功！");
            }
            
        }

        private void ReturnButton_Click(object sender, EventArgs e)
        {
            foreach (Control c in this.Controls)
            {
                if (c.Tag.ToString() == "1" || c.Tag.ToString() == "2")
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
            //Form1 f1 = new Form1();
            //f1.arrayServer = LocalAdapter.Text;
            //f1.ntpServer = PLC_IP.Text;
            //f1.powerServer = Power_Amplifier.Text;
            //f1.speed = Convert.ToSingle(Speed.Text);
            //f1.num_of_mouth = Convert.ToInt32(MouthNum.Text);
            //f1.length_belt = Convert.ToInt32(BeltLength.Text);
            //f1.length_linearray = Convert.ToInt32(ArrayLength.Text);
            //f1.SOD = Convert.ToInt32(SOD.Text);
            //f1.SDD = Convert.ToInt32(SDD.Text);
            //f1.gap = Convert.ToInt32(gap.Text);
            //f1.SetSetting();
            //f1.ShowDialog();
        }

        private void TestButton_Click(object sender, EventArgs e)
        {
            TestForm f1 = new TestForm();
            //f1.arrayServer = LocalAdapter.Text;
            //f1.ntpServer = PLC_IP.Text;
            //f1.powerServer = Power_Amplifier.Text;
            f1.speed = Convert.ToSingle(Speed.Text);
            f1.num_of_mouth = Convert.ToInt32(MouthNum.Text);
            f1.length_belt = Convert.ToInt32(BeltLength.Text);
            f1.length_linearray = Convert.ToInt32(ArrayLength.Text);
            f1.SOD = Convert.ToInt32(SOD.Text);
            f1.SDD = Convert.ToInt32(SDD.Text);
            f1.gap = Convert.ToInt32(gap.Text);
            f1.version = Version.Text;
            f1.Show();
        }

        private void StartMenu_Load(object sender, EventArgs e)
        {
            Sunisoft.IrisSkin.SkinEngine skinEngine1 = new Sunisoft.IrisSkin.SkinEngine();
            //skinEngine1.SkinFile = System.Environment.CurrentDirectory + "\\Skins\\MacOS.ssk";
        }

    }
}
