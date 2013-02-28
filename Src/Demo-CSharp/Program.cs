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


            NuiFactory.Init();
            NuiFactory.SetAutoPoll(true);

            DotNetVector hand = DotNetVector.joint(11);
            DotNetVector head = DotNetVector.joint(3);


            DotNetScalar x = DotNetScalar.x(hand);
            DotNetScalar y = DotNetScalar.y(head);
            DotNetScalar diff = x - y;

            hand.OnChange += () => Console.WriteLine("{0} - {1} = {2}", x.Value, y.Value, diff.Value);

            NuiFactory.Pause();
            NuiFactory.SetAutoPoll(false);
            Console.WriteLine("{0} - {1} = {2}", x.Value, y.Value, diff.Value);
            NuiFactory.Pause();
            //Console.ReadLine();
        }
    }
}
