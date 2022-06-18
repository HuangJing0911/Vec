using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace XEthernetDemo
{
    public partial class ParamSettingForm : Form
    {
        public int nozzleNum;
        public int beltLength;
        public int arrayLength;
        public int beltVelocity;

        public ParamSettingForm()
        {
            InitializeComponent();
        }
        public ParamSettingForm(int _nozzleNum,int _beltLength,
            int _arrayLength,int _beltVelocity)
        {
            nozzleNum = _nozzleNum;
            beltLength = _beltLength;
            arrayLength = _arrayLength;
            beltLength = _beltLength;
            nozzleNumTextBox.Text = nozzleNum.ToString();
            beltLengthTextBox.Text = beltLength.ToString();
            arrayLengthTextBox.Text = arrayLength.ToString();
            beltVelocityTextBox.Text = beltVelocity.ToString();
        }

        private void settingButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }
    }
}
