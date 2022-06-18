namespace XEthernetDemo
{
    partial class ParamSettingForm
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
            this.settingButton = new System.Windows.Forms.Button();
            this.nozzleNumTextBox = new System.Windows.Forms.TextBox();
            this.nozzleNumLabel = new System.Windows.Forms.Label();
            this.beltLengthLabel = new System.Windows.Forms.Label();
            this.beltLengthTextBox = new System.Windows.Forms.TextBox();
            this.arrayLengthLabel = new System.Windows.Forms.Label();
            this.arrayLengthTextBox = new System.Windows.Forms.TextBox();
            this.beltVelocityLabel = new System.Windows.Forms.Label();
            this.beltVelocityTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // settingButton
            // 
            this.settingButton.Location = new System.Drawing.Point(552, 321);
            this.settingButton.Name = "settingButton";
            this.settingButton.Size = new System.Drawing.Size(94, 40);
            this.settingButton.TabIndex = 0;
            this.settingButton.Text = "设置";
            this.settingButton.UseVisualStyleBackColor = true;
            this.settingButton.Click += new System.EventHandler(this.settingButton_Click);
            // 
            // nozzleNumTextBox
            // 
            this.nozzleNumTextBox.Location = new System.Drawing.Point(14, 24);
            this.nozzleNumTextBox.Name = "nozzleNumTextBox";
            this.nozzleNumTextBox.Size = new System.Drawing.Size(100, 21);
            this.nozzleNumTextBox.TabIndex = 1;
            this.nozzleNumTextBox.Text = "198";
            // 
            // nozzleNumLabel
            // 
            this.nozzleNumLabel.AutoSize = true;
            this.nozzleNumLabel.Location = new System.Drawing.Point(12, 9);
            this.nozzleNumLabel.Name = "nozzleNumLabel";
            this.nozzleNumLabel.Size = new System.Drawing.Size(65, 12);
            this.nozzleNumLabel.TabIndex = 2;
            this.nozzleNumLabel.Text = "喷嘴数量：";
            // 
            // beltLengthLabel
            // 
            this.beltLengthLabel.AutoSize = true;
            this.beltLengthLabel.Location = new System.Drawing.Point(10, 60);
            this.beltLengthLabel.Name = "beltLengthLabel";
            this.beltLengthLabel.Size = new System.Drawing.Size(89, 12);
            this.beltLengthLabel.TabIndex = 4;
            this.beltLengthLabel.Text = "皮带长度(mm)：";
            // 
            // beltLengthTextBox
            // 
            this.beltLengthTextBox.Location = new System.Drawing.Point(12, 75);
            this.beltLengthTextBox.Name = "beltLengthTextBox";
            this.beltLengthTextBox.Size = new System.Drawing.Size(100, 21);
            this.beltLengthTextBox.TabIndex = 3;
            this.beltLengthTextBox.Text = "1016";
            // 
            // arrayLengthLabel
            // 
            this.arrayLengthLabel.AutoSize = true;
            this.arrayLengthLabel.Location = new System.Drawing.Point(10, 110);
            this.arrayLengthLabel.Name = "arrayLengthLabel";
            this.arrayLengthLabel.Size = new System.Drawing.Size(89, 12);
            this.arrayLengthLabel.TabIndex = 6;
            this.arrayLengthLabel.Text = "线阵长度(mm)：";
            // 
            // arrayLengthTextBox
            // 
            this.arrayLengthTextBox.Location = new System.Drawing.Point(12, 125);
            this.arrayLengthTextBox.Name = "arrayLengthTextBox";
            this.arrayLengthTextBox.Size = new System.Drawing.Size(100, 21);
            this.arrayLengthTextBox.TabIndex = 5;
            this.arrayLengthTextBox.Text = "1120";
            // 
            // beltVelocityLabel
            // 
            this.beltVelocityLabel.AutoSize = true;
            this.beltVelocityLabel.Location = new System.Drawing.Point(10, 167);
            this.beltVelocityLabel.Name = "beltVelocityLabel";
            this.beltVelocityLabel.Size = new System.Drawing.Size(95, 12);
            this.beltVelocityLabel.TabIndex = 8;
            this.beltVelocityLabel.Text = "皮带速度(m/s)：";
            // 
            // beltVelocityTextBox
            // 
            this.beltVelocityTextBox.Location = new System.Drawing.Point(12, 182);
            this.beltVelocityTextBox.Name = "beltVelocityTextBox";
            this.beltVelocityTextBox.Size = new System.Drawing.Size(100, 21);
            this.beltVelocityTextBox.TabIndex = 7;
            this.beltVelocityTextBox.Text = "3";
            // 
            // ParamSetForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(658, 373);
            this.Controls.Add(this.beltVelocityLabel);
            this.Controls.Add(this.beltVelocityTextBox);
            this.Controls.Add(this.arrayLengthLabel);
            this.Controls.Add(this.arrayLengthTextBox);
            this.Controls.Add(this.beltLengthLabel);
            this.Controls.Add(this.beltLengthTextBox);
            this.Controls.Add(this.nozzleNumLabel);
            this.Controls.Add(this.nozzleNumTextBox);
            this.Controls.Add(this.settingButton);
            this.Name = "ParamSetForm";
            this.Text = "paramSetForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button settingButton;
        private System.Windows.Forms.TextBox nozzleNumTextBox;
        private System.Windows.Forms.Label nozzleNumLabel;
        private System.Windows.Forms.Label beltLengthLabel;
        private System.Windows.Forms.TextBox beltLengthTextBox;
        private System.Windows.Forms.Label arrayLengthLabel;
        private System.Windows.Forms.TextBox arrayLengthTextBox;
        private System.Windows.Forms.Label beltVelocityLabel;
        private System.Windows.Forms.TextBox beltVelocityTextBox;
    }
}