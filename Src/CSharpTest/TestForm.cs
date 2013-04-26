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
            byte[] bytes = Nui.DepthBytes;
            Bitmap b = new Bitmap(640, 480, System.Drawing.Imaging.PixelFormat.Format16bppGrayScale);
            var x = b.LockBits(new Rectangle(0, 0, 640, 480), System.Drawing.Imaging.ImageLockMode.WriteOnly, System.Drawing.Imaging.PixelFormat.Format16bppGrayScale);
            Marshal.Copy(bytes, 0, x.Scan0, bytes.Length);
            b.UnlockBits(x);

            b.Save("../Test.jpg");

            BeginInvoke(new Action(() => {
                displayPanel.Image = b;
            }));
        }
    }
}
