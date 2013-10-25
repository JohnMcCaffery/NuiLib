namespace CShartTest {
    partial class TestForm {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.displayPanel = new System.Windows.Forms.PictureBox();
            this.initialiseButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.displayPanel)).BeginInit();
            this.SuspendLayout();
            // 
            // displayPanel
            // 
            this.displayPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.displayPanel.Location = new System.Drawing.Point(0, 0);
            this.displayPanel.Name = "displayPanel";
            this.displayPanel.Size = new System.Drawing.Size(640, 480);
            this.displayPanel.TabIndex = 0;
            this.displayPanel.TabStop = false;
            // 
            // initialiseButton
            // 
            this.initialiseButton.Location = new System.Drawing.Point(13, 13);
            this.initialiseButton.Name = "initialiseButton";
            this.initialiseButton.Size = new System.Drawing.Size(75, 23);
            this.initialiseButton.TabIndex = 1;
            this.initialiseButton.Text = "Initialise";
            this.initialiseButton.UseVisualStyleBackColor = true;
            this.initialiseButton.Click += new System.EventHandler(this.initialiseButton_Click);
            // 
            // TestForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(640, 480);
            this.Controls.Add(this.initialiseButton);
            this.Controls.Add(this.displayPanel);
            this.Name = "TestForm";
            this.Text = "TestForm";
            ((System.ComponentModel.ISupportInitialize)(this.displayPanel)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox displayPanel;
        private System.Windows.Forms.Button initialiseButton;
    }
}