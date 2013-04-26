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
            */

            byte[] bytes = Nui.DepthBytes;
            //Bitmap b = new Bitmap(640, 480, System.Drawing.Imaging.PixelFormat.Format16bppRgb565);
            //Bitmap b = new Bitmap(640, 480, System.Drawing.Imaging.PixelFormat.Format16bppGrayScale);
            //Bitmap b = new Bitmap(640, 480, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
            Bitmap b = new Bitmap(640, 480, System.Drawing.Imaging.PixelFormat.Format64bppArgb);

            /*
            double max = (32.0 / Math.Pow(2, 16));
            for (int i = 0; i < bytes.Length; i += 2) {
                uint pixel = (uint) (bytes[i] * 256) + bytes[i + 1];
                //Console.WriteLine(Convert.ToString(pixel, 16) + " = " + Convert.ToString(bytes[i], 16) + " + " + Convert.ToString(bytes[i+1], 16));
                uint component = (uint) ((double) pixel * max);
                uint outpix = (component << 10) | (component << 5) | (component << 0);
                bytes[i] = (byte) (outpix >> 8);
                bytes[i+1] = (byte) outpix;

                //Console.WriteLine(Convert.ToString(component, 2).PadLeft(5, '0') + " = " + Convert.ToString(outpix, 2).PadLeft(16, '0') + " / " + Convert.ToString(bytes[i], 2).PadLeft(8, '0') + "+" + Convert.ToString(bytes[i+1], 2).PadLeft(8, '0'));
            }
            */

            var x = b.LockBits(new Rectangle(0, 0, 640, 480), System.Drawing.Imaging.ImageLockMode.WriteOnly, System.Drawing.Imaging.PixelFormat.Format64bppArgb);
            //var x = b.LockBits(new Rectangle(0, 0, 640, 480), System.Drawing.Imaging.ImageLockMode.WriteOnly, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
            //var x = b.LockBits(new Rectangle(0, 0, 640, 480), System.Drawing.Imaging.ImageLockMode.WriteOnly, System.Drawing.Imaging.PixelFormat.Format16bppGrayScale);
            //var x = b.LockBits(new Rectangle(0, 0, 640, 480), System.Drawing.Imaging.ImageLockMode.WriteOnly, System.Drawing.Imaging.PixelFormat.Format16bppRgb565);

            Marshal.Copy(bytes, 0, x.Scan0, bytes.Length);
            b.UnlockBits(x);

            BeginInvoke(new Action(() => {
                displayPanel.Image = b;
            }));
        }
    }
}
