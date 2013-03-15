using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NuiLibDotNet;

namespace CShartTest {
    class Program {
        static void Main(string[] args) {
            //Test.Program.Main(args);

            Vector mPointStart = Vector.Create("PointStart", 0f, 0f, 0f);
            Vector mPointDir = Vector.Create("PointDir", 0f, 0f, 0f);

			Nui.Init();
            Nui.SetAutoPoll(true);
            Vector pointEnd = Nui.joint(Nui.Hand_Right);
            mPointStart = Nui.joint(Nui.Shoulder_Right);
            mPointDir = mPointStart - pointEnd;


            //Correct behaviour:
            //      Top Left: -1, 1, 0
            //      Normal: 0, 0, 1
            Vector mPlaneTopLeft = Vector.Create("TopLeft", 1, 1f, 0f);
            Vector mPlaneNormal = Vector.Create("PlaneNormal", 0f, 0f, -1f);
            Scalar mW = Scalar.Create("W", 2f);
            Scalar mH = Scalar.Create("H", 2f);

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
                if (mX.Value > 0 && mX.Value < mW.Value)
                    Console.WriteLine(mX.Value);
            };

            Nui.Tick += () => {
                Console.WriteLine("Nui Tick");
            };

            Nui.SkeletonFound += () => {
                Console.WriteLine("Skeleton Found");
            };
            Nui.SkeletonLost += () => {
                Console.WriteLine("Skeleton Lost");
            };
            Nui.SkeletonSwitched += () => {
                Console.WriteLine("Skeleton Switched");
            };

            mW.Value = 3f;


            Nui.Pause();

            //Nui.Close();
        }

        private static Vector make() {
            Vector v1 = Vector.Create(0f);
            Vector v2 = Vector.Create(1f);
            Vector v3 = v1 + v2;
            return v1 + v3;
        }
    }
}
