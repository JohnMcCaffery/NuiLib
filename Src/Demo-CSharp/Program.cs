using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NuiLibDotNet;
using System.Windows.Forms;
using System.Threading;
using System.Net;
using OpenMetaverse;
using System.Drawing;

namespace Demo_CSharp {
    class Program {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        [STAThread]
        static void Main(string[] args) {
            ChimeraOutput.RunChimera.Main();
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
        }

        private static Vector make() {
            Vector v1 = Vector.Create(0f);
            Vector v2 = Vector.Create(1f);
            Vector v3 = v1 + v2;
            return v1 + v3;
        }
    }
}
