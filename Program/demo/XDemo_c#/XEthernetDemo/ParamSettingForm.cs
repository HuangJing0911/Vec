using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Serialization;
using System.IO;

namespace XEthernetDemo
{
    public partial class ParamSettingForm : Form
    {
        public struct VecParams
        {
            public int _nozzleNum;
            public int _beltLength;
            public int _arrayLength;
            public int _beltVelocity;
            //public double[] _powerThreshold;
            public int _minItemArea;
            public int _itemThreshold;
        }
        VecParams parms = new VecParams();
        public int nozzleNum { get { return parms._nozzleNum; }set { parms._nozzleNum = value; } }
        public int beltLength { get { return parms._beltLength; } set { parms._beltLength = value; } }
        public int arrayLength { get { return parms._arrayLength; }set { parms._arrayLength = value; } }
        public int beltVelocity { get { return parms._beltVelocity; } set { parms._beltVelocity = value; } }
        //public double[] powerThreshold { get { return parms._powerThreshold; } set { parms._powerThreshold = value; } }
        public int minItemArea { get { return parms._minItemArea; } set { parms._minItemArea = value; } }
        public int itemThreshold { get { return parms._itemThreshold; } set { parms._itemThreshold = value; } }


        public ParamSettingForm()
        {
            InitializeComponent();
        }
        public ParamSettingForm(int _nozzleNum,int _beltLength,
            int _arrayLength,int _beltVelocity,int _powerThreshold,
            int _minItemArea,int _itemThreshold)
        {
            InitializeComponent();
            nozzleNum = _nozzleNum;
            beltLength = _beltLength;
            arrayLength = _arrayLength;
            beltLength = _beltVelocity;
            //powerThreshold = _powerThreshold;
            minItemArea = _minItemArea;
            itemThreshold = _itemThreshold;

            nozzleNumTextBox.Text = nozzleNum.ToString();
            beltLengthTextBox.Text = beltLength.ToString();
            arrayLengthTextBox.Text = arrayLength.ToString();
            beltVelocityTextBox.Text = beltVelocity.ToString();
            //powerThresholdTextBox.Text = powerThreshold.ToString();
            minItemAreaTextBox.Text = minItemArea.ToString();
            itemThresholdTextBox.Text = itemThreshold.ToString();

        }

        private void settingButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Multiselect = false;
            dialog.Title = "参数";
            dialog.Filter = "(参数文件*.xml)|*.xml";
            string s_dialogShowDialog = dialog.ShowDialog().ToString();
            string file;
            if (s_dialogShowDialog != "OK")
                return;
            file = dialog.FileName;
            serializerMe(file);
            DialogResult = DialogResult.OK;
            Close();
        }

        private void serializerMe(string file)
        {
            XmlSerializer xmlSerializer = new XmlSerializer(typeof(VecParams));
            FileStream fs = new FileStream(file, FileMode.Create);
            xmlSerializer.Serialize(fs, parms);
            fs.Close();
        }

        private void ParamSettingForm_Load(object sender, EventArgs e)
        {
            //OpenFileDialog dialog = new OpenFileDialog();
            //dialog.Multiselect = false;
            //dialog.Title = "选择工程";
            //dialog.Filter = "(工程文件*.xml)|*.xml";
            //if (dialog.ShowDialog() == DialogResult.OK)
            //{
            //    XmlSerializer ser = new XmlSerializer(typeof(VecParams));
            //    FileStream fs = new FileStream(dialog.FileName, FileMode.Open, FileAccess.Read);
            //    parms = (VecParams)(ser.Deserialize(fs));
            //}
            nozzleNum = Convert.ToInt32(nozzleNumTextBox.Text);
            beltLength = Convert.ToInt32(beltLengthTextBox.Text);
            arrayLength = Convert.ToInt32(arrayLengthTextBox.Text);
            beltLength = Convert.ToInt32(beltLengthTextBox.Text);
            //powerThreshold = Convert.ToDouble(powerThresholdTextBox.Text);
            minItemArea = Convert.ToInt32(minItemAreaTextBox.Text);
            itemThreshold = Convert.ToInt32(itemThresholdTextBox.Text);

        }
    }
}
