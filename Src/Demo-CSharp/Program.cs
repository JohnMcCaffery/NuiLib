using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NuiLibDotNet;
using System.Windows.Forms;
using System.Threading;
using System.Net;
using System.Drawing;

namespace Demo_CSharp {
    class Program {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        [STAThread]
        static void Main(string[] args) {

            Vector mPointStart = Vector.Create("PointStart", 0f, 0f, 0f);
            Vector mPointDir = Vector.Create("PointDir", 0f, 0f, 0f);
            Vector mPlaneTopLeft = Vector.Create("PlanePoint", 1f, 1f, 0f);
            Vector mPlaneNormal = Vector.Create("PlaneNormal", 0f, 0f, 1f);
            Scalar mW = Scalar.Create("W", 1f);
            Scalar mH = Scalar.Create("H", 1f);



            Vector vertical = Vector.Create(0f, 1f, 0f); // Vertical
            //Calculate the intersection of the plane defined by the point mPlaneTopLeft and the normal mPlaneNormal and the line defined by the point mPointStart and the direction mPointDir.
            Vector mIntersection = Nui.intersect(mPlaneTopLeft, Nui.normalize(mPlaneNormal), mPointStart, Nui.normalize(mPointDir));
            //Calculate a vector that represents the orientation of the top of the coordinator.
            Vector mTop = Nui.scale(Nui.cross(vertical, mPlaneNormal), mW);
            //Calculate a vector that represents the orientation of the side of the coordinator.
            Vector mSide = Nui.scale(Nui.cross(mPlaneNormal, mTop), mH);

            //Calculate the vector (running along the plane) between the top left corner and the point of intersection.
            Vector diff = mIntersection - mPlaneTopLeft;

            //Project the diff line onto the top and side vectors to get x and y values.
            Scalar mX = Nui.project(diff, mTop);
            Scalar mY = Nui.project(diff, mSide);

            mX.OnChange += () => {
                Console.WriteLine(mX.Value);
            };

            mPointStart.Set(1f, 0f, 0f);

            Nui.Poll();


            /*

            Scalar scalar = Scalar.Create(10f);
            Console.WriteLine("{0}", scalar.Value);
            scalar.OnChange += () => {
                Console.WriteLine("{0}", scalar.Value);
            };
            scalar.Value = 20f;

            Scalar namedScalar = Scalar.Create("Name", 0f);
            Scalar unnamedScalar = Scalar.Create(0f);

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
