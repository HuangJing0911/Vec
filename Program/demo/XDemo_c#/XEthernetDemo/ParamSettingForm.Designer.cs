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
            this.powerThresholdTextBox = new System.Windows.Forms.TextBox();
            this.powerThresholdLabel = new System.Windows.Forms.Label();
            this.minItemAreaLabel = new System.Windows.Forms.Label();
            this.minItemAreaTextBox = new System.Windows.Forms.TextBox();
            this.itemThresholdLabel = new System.Windows.Forms.Label();
            this.itemThresholdTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // settingButton
            // 
            this.settingButton.Location = new System.Drawing.Point(736, 401);
            this.settingButton.Margin = new System.Windows.Forms.Padding(4);
            this.settingButton.Name = "settingButton";
            this.settingButton.Size = new System.Drawing.Size(125, 50);
            this.settingButton.TabIndex = 0;
            this.settingButton.Text = "设置";
            this.settingButton.UseVisualStyleBackColor = true;
            this.settingButton.Click += new System.EventHandler(this.settingButton_Click);
            // 
            // nozzleNumTextBox
            // 
            this.nozzleNumTextBox.Location = new System.Drawing.Point(19, 30);
            this.nozzleNumTextBox.Margin = new System.Windows.Forms.Padding(4);
            this.nozzleNumTextBox.Name = "nozzleNumTextBox";
            this.nozzleNumTextBox.Size = new System.Drawing.Size(132, 25);
            this.nozzleNumTextBox.TabIndex = 1;
            this.nozzleNumTextBox.Text = "198";
            // 
            // nozzleNumLabel
            // 
            this.nozzleNumLabel.AutoSize = true;
            this.nozzleNumLabel.Location = new System.Drawing.Point(16, 11);
            this.nozzleNumLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.nozzleNumLabel.Name = "nozzleNumLabel";
            this.nozzleNumLabel.Size = new System.Drawing.Size(82, 15);
            this.nozzleNumLabel.TabIndex = 2;
            this.nozzleNumLabel.Text = "功放阈值：";
            // 
            // beltLengthLabel
            // 
            this.beltLengthLabel.AutoSize = true;
            this.beltLengthLabel.Location = new System.Drawing.Point(13, 75);
            this.beltLengthLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.beltLengthLabel.Name = "beltLengthLabel";
            this.beltLengthLabel.Size = new System.Drawing.Size(114, 15);
            this.beltLengthLabel.TabIndex = 4;
            this.beltLengthLabel.Text = "皮带长度(mm)：";
            // 
            // beltLengthTextBox
            // 
            this.beltLengthTextBox.Location = new System.Drawing.Point(16, 94);
            this.beltLengthTextBox.Margin = new System.Windows.Forms.Padding(4);
            this.beltLengthTextBox.Name = "beltLengthTextBox";
            this.beltLengthTextBox.Size = new System.Drawing.Size(132, 25);
            this.beltLengthTextBox.TabIndex = 3;
            this.beltLengthTextBox.Text = "1016";
            // 
            // arrayLengthLabel
            // 
            this.arrayLengthLabel.AutoSize = true;
            this.arrayLengthLabel.Location = new System.Drawing.Point(13, 138);
            this.arrayLengthLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.arrayLengthLabel.Name = "arrayLengthLabel";
            this.arrayLengthLabel.Size = new System.Drawing.Size(114, 15);
            this.arrayLengthLabel.TabIndex = 6;
            this.arrayLengthLabel.Text = "线阵长度(mm)：";
            // 
            // arrayLengthTextBox
            // 
            this.arrayLengthTextBox.Location = new System.Drawing.Point(16, 156);
            this.arrayLengthTextBox.Margin = new System.Windows.Forms.Padding(4);
            this.arrayLengthTextBox.Name = "arrayLengthTextBox";
            this.arrayLengthTextBox.Size = new System.Drawing.Size(132, 25);
            this.arrayLengthTextBox.TabIndex = 5;
            this.arrayLengthTextBox.Text = "1120";
            // 
            // beltVelocityLabel
            // 
            this.beltVelocityLabel.AutoSize = true;
            this.beltVelocityLabel.Location = new System.Drawing.Point(13, 209);
            this.beltVelocityLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.beltVelocityLabel.Name = "beltVelocityLabel";
            this.beltVelocityLabel.Size = new System.Drawing.Size(122, 15);
            this.beltVelocityLabel.TabIndex = 8;
            this.beltVelocityLabel.Text = "皮带速度(m/s)：";
            // 
            // beltVelocityTextBox
            // 
            this.beltVelocityTextBox.Location = new System.Drawing.Point(16, 228);
            this.beltVelocityTextBox.Margin = new System.Windows.Forms.Padding(4);
            this.beltVelocityTextBox.Name = "beltVelocityTextBox";
            this.beltVelocityTextBox.Size = new System.Drawing.Size(132, 25);
            this.beltVelocityTextBox.TabIndex = 7;
            this.beltVelocityTextBox.Text = "3";
            // 
            // powerThresholdTextBox
            // 
            this.powerThresholdTextBox.Location = new System.Drawing.Point(272, 30);
            this.powerThresholdTextBox.Margin = new System.Windows.Forms.Padding(4);
            this.powerThresholdTextBox.Name = "powerThresholdTextBox";
            this.powerThresholdTextBox.Size = new System.Drawing.Size(132, 25);
            this.powerThresholdTextBox.TabIndex = 9;
            this.powerThresholdTextBox.Text = "0.8";
            // 
            // powerThresholdLabel
            // 
            this.powerThresholdLabel.AutoSize = true;
            this.powerThresholdLabel.Location = new System.Drawing.Point(269, 9);
            this.powerThresholdLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.powerThresholdLabel.Name = "powerThresholdLabel";
            this.powerThresholdLabel.Size = new System.Drawing.Size(122, 15);
            this.powerThresholdLabel.TabIndex = 10;
            this.powerThresholdLabel.Text = "功放阈值(0~1)：";
            // 
            // minItemAreaLabel
            // 
            this.minItemAreaLabel.AutoSize = true;
            this.minItemAreaLabel.Location = new System.Drawing.Point(269, 75);
            this.minItemAreaLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.minItemAreaLabel.Name = "minItemAreaLabel";
            this.minItemAreaLabel.Size = new System.Drawing.Size(112, 15);
            this.minItemAreaLabel.TabIndex = 11;
            this.minItemAreaLabel.Text = "物块最小面积：";
            // 
            // minItemAreaTextBox
            // 
            this.minItemAreaTextBox.Location = new System.Drawing.Point(272, 94);
            this.minItemAreaTextBox.Margin = new System.Windows.Forms.Padding(4);
            this.minItemAreaTextBox.Name = "minItemAreaTextBox";
            this.minItemAreaTextBox.Size = new System.Drawing.Size(132, 25);
            this.minItemAreaTextBox.TabIndex = 12;
            this.minItemAreaTextBox.Text = "20";
            // 
            // itemThresholdLabel
            // 
            this.itemThresholdLabel.AutoSize = true;
            this.itemThresholdLabel.Location = new System.Drawing.Point(269, 138);
            this.itemThresholdLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.itemThresholdLabel.Name = "itemThresholdLabel";
            this.itemThresholdLabel.Size = new System.Drawing.Size(168, 15);
            this.itemThresholdLabel.TabIndex = 13;
            this.itemThresholdLabel.Text = "功放物块阈值(0~255)：";
            // 
            // itemThresholdTextBox
            // 
            this.itemThresholdTextBox.Location = new System.Drawing.Point(272, 157);
            this.itemThresholdTextBox.Margin = new System.Windows.Forms.Padding(4);
            this.itemThresholdTextBox.Name = "itemThresholdTextBox";
            this.itemThresholdTextBox.Size = new System.Drawing.Size(132, 25);
            this.itemThresholdTextBox.TabIndex = 14;
            this.itemThresholdTextBox.Text = "80";
            // 
            // ParamSettingForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(877, 466);
            this.Controls.Add(this.itemThresholdTextBox);
            this.Controls.Add(this.itemThresholdLabel);
            this.Controls.Add(this.minItemAreaTextBox);
            this.Controls.Add(this.minItemAreaLabel);
            this.Controls.Add(this.powerThresholdLabel);
            this.Controls.Add(this.powerThresholdTextBox);
            this.Controls.Add(this.beltVelocityLabel);
            this.Controls.Add(this.beltVelocityTextBox);
            this.Controls.Add(this.arrayLengthLabel);
            this.Controls.Add(this.arrayLengthTextBox);
            this.Controls.Add(this.beltLengthLabel);
            this.Controls.Add(this.beltLengthTextBox);
            this.Controls.Add(this.nozzleNumLabel);
            this.Controls.Add(this.nozzleNumTextBox);
            this.Controls.Add(this.settingButton);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "ParamSettingForm";
            this.Text = "paramSetForm";
            this.Load += new System.EventHandler(this.ParamSettingForm_Load);
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
        private System.Windows.Forms.TextBox powerThresholdTextBox;
        private System.Windows.Forms.Label powerThresholdLabel;
        private System.Windows.Forms.Label minItemAreaLabel;
        private System.Windows.Forms.TextBox minItemAreaTextBox;
        private System.Windows.Forms.Label itemThresholdLabel;
        private System.Windows.Forms.TextBox itemThresholdTextBox;
    }
}