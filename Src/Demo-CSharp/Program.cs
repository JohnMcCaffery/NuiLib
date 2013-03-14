using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NuiLibDotNet;
using System.Windows.Forms;
using KinectLib;
using System.Threading;
using UtilLib;
using FlythroughLib;
using System.Net;
using OpenMetaverse;
using ChimeraLib;
using System.Drawing;

namespace Demo_CSharp {
    class Program {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        [STAThread]
        static void Main(string[] args) {
            /*
            Scalar scalar = Scalar.Create(10f);
            Console.WriteLine("{0}", scalar.Value);
            scalar.OnChange += () => {
                Console.WriteLine("{0}", scalar.Value);
            };
            scalar.Value = 20f;

            Vector vector = Vector.Create(1f, 2f, 3f);
            Console.WriteLine("<{0},{1},{2}>", vector.X, vector.Y, vector.Z);
            vector.OnChange += () => {
                Console.WriteLine("<{0},{1},{2}>", vector.X, vector.Y, vector.Z);
            };
            vector.Set(20f, 15, 12f);


            Nui.Init();
            Nui.SetAutoPoll(true);

            Vector hand = Nui.joint(Nui.Hand_Right);
            Vector head = Nui.joint(Nui.Head);


            Scalar x = Nui.x(hand);
            Scalar y = Nui.y(head);
            Scalar diff = x - y;
            Vector diffV = hand - head;
            Condition equal = x == y;
            Condition notEqual = x != y;
            Condition not = !equal;
            //Condition or = equal || not;
            Condition or = Condition.op_LogicalOr(equal, not);
            //Condition and = equal && not;
            Condition and = Condition.op_LogicalAnd(equal, not);

            Scalar tracker = Nui.tracker("Test", 1f, 0f, 0f);

            hand.OnChange += () => Console.WriteLine("{0} - {1} = {2}", x.Value, y.Value, diff.Value);

            Nui.Pause();
            Nui.SetAutoPoll(false);
            Console.WriteLine("{0} - {1} = {2}", x.Value, y.Value, diff.Value);
            Nui.Pause();
            //Console.ReadLine();
            */
            /*
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            KinectManager kinectManager = new KinectManager();
            //kinectManager.Init(master, flythrough);
            kinectManager.SurfaceAdded += surface => {
                InputWindow frame = new InputWindow();
                frame.Init(kinectManager, surface);
                foreach (ActiveArea.Data area in kinectManager.ActiveAreas)
                    frame.AddActiveArea(new ActiveArea(surface, area));

                Thread t = new Thread(() => Application.Run(frame));
                t.Name = "Frame Thread";
                t.SetApartmentState(ApartmentState.STA);
                t.Start();
            };

            Action evt = () => Console.WriteLine("Event triggered.");
            kinectManager.AddActiveArea(new ActiveArea.Data("C:\\Users\\Iain\\Desktop\\Helmsdale Demo - 18-2-2013\\100_2345.JPG", new RectangleF(.1f, .1f, .3f, .3f), evt));
            kinectManager.AddActiveArea(new ActiveArea.Data("C:\\Users\\Iain\\Desktop\\Helmsdale Demo - 18-2-2013\\100_2344.JPG", new RectangleF(.6f, .1f, .3f, .3f), evt));
            kinectManager.AddActiveArea(new ActiveArea.Data("C:\\Users\\Iain\\Desktop\\Helmsdale Demo - 18-2-2013\\BroraSEW.jpg", new RectangleF(.1f, .6f, .3f, .3f), evt));
            kinectManager.AddActiveArea(new ActiveArea.Data("C:\\Users\\Iain\\Desktop\\Helmsdale Demo - 18-2-2013\\s_BBB11_Aug-17-11_Brora-day 16 074.jpg", new RectangleF(.6f, .6f, .3f, .3f), evt));

            Window window = new Window("Test Window");
            window.ScreenPosition = Vector3.Zero;
            window.RotationOffset.Yaw = 0f;
            kinectManager.AddWindow(window);
             * */

            Vector vector = Vector.Create(1f, 2f, 3f);
            vector.OnChange += () => Console.WriteLine("Original vector changed.");
            Scalar o = make(vector);
            o.OnChange += () => Console.WriteLine("Output scalar changed.");
            GC.Collect();
            vector.Set(4f, 5f, 6f);
            Nui.Poll();
            Console.WriteLine("Completed.");
        }

        static Scalar make(Vector start) {
            return Nui.x(Nui.normalize(start));
        }

        private static Vector make() {
            Vector v1 = Vector.Create(0f);
            Vector v2 = Vector.Create(1f);
            Vector v3 = v1 + v2;
            return v1 + v3;
        }
    }
}
