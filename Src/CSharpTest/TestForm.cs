using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NuiLibDotNet;
using System.Runtime.InteropServices;

namespace CShartTest {
    public partial class TestForm : Form {
        public TestForm() {
            InitializeComponent();

            Nui.Tick += new ChangeDelegate(Nui_Tick);
        }

        void Nui_Tick() {
            /*
            byte[] bytes = Nui.ColourBytes;
            Bitmap b = new Bitmap(640, 480, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
            var x = b.LockBits(new Rectangle(0, 0, 640, 480), System.Drawing.Imaging.ImageLockMode.WriteOnly, System.Drawing.Imaging.PixelFormat.Format32bppRgb);

            byte[] bytes = Nui.DepthBytes;
            Bitmap b = new Bitmap(640, 480, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            var x = b.LockBits(new Rectangle(0, 0, 640, 480), System.Drawing.Imaging.ImageLockMode.WriteOnly, System.Drawing.Imaging.PixelFormat.Format24bppRgb);

            Marshal.Copy(bytes, 0, x.Scan0, bytes.Length);
            b.UnlockBits(x);
            */

            BeginInvoke(new Action(() => {
                displayPanel.Image = Nui.ColourFrame;
            }));
        }

        private void initialiseButton_Click(object sender, EventArgs e) {
            if (Nui.Initialised) {
                initialiseButton.Text = "Initialise";
                Nui.Uninitialise();
                Console.WriteLine(Nui.State);
            } else {
                initialiseButton.Text = "Uninitialise";
                if (Nui.Init())
                    Console.WriteLine("Device initialised");
                else
                    Console.WriteLine("Problem initialising: " + Nui.State);
            }
        }
    }
}
