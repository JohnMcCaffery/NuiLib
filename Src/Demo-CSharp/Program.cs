using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NuiLibDotNet;

namespace Demo_CSharp {
    class Program {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args) {
            /*
            DotNetScalar scalar = DotNetScalar.Create(10f);
            Console.WriteLine("{0}", scalar.Value);
            scalar.OnChange += () => {
                Console.WriteLine("{0}", scalar.Value);
            };
            scalar.Value = 20f;

            DotNetVector vector = DotNetVector.Create(1f, 2f, 3f);
            Console.WriteLine("<{0},{1},{2}>", vector.X, vector.Y, vector.Z);
            vector.OnChange += () => {
                Console.WriteLine("<{0},{1},{2}>", vector.X, vector.Y, vector.Z);
            };
            vector.Set(20f, 15, 12f);
            */


            Nui.Init();
            Nui.SetAutoPoll(true);

            DotNetVector hand = Nui.joint(Nui.Hand_Right);
            DotNetVector head = Nui.joint(Nui.Head);


            DotNetScalar x = Nui.x(hand);
            DotNetScalar y = Nui.y(head);
            DotNetScalar diff = x - y;
            DotNetVector diffV = hand - head;
            DotNetCondition equal = x == y;
            DotNetCondition not = x != y;
            //DotNetCondition or = equal || not;
            //DotNetCondition and = equal && not;

            DotNetScalar tracker = Nui.tracker("Test", 1f, 0f, 0f);

            hand.OnChange += () => Console.WriteLine("{0} - {1} = {2}", x.Value, y.Value, diff.Value);

            Nui.Pause();
            Nui.SetAutoPoll(false);
            Console.WriteLine("{0} - {1} = {2}", x.Value, y.Value, diff.Value);
            Nui.Pause();
            //Console.ReadLine();
        }
    }
}
