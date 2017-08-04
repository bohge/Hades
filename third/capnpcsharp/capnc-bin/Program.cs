using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CapnProto;
using CapnProto.Schema;
using System.IO;

namespace capnc_bin
{
    class Program
    {
        static void Main(string[] args)
        {
            string source = args[0];
            string destination = args[1];
            using (var msg = Message.Load(source))
            {
                msg.ReadNext();
                var req = (CodeGeneratorRequest)msg.Root;
                using (var sw = new StringWriter())
                {
                    var codeWriter = new CSharpStructWriter(sw, req.nodes, "CapnProtoStruct");
                    req.GenerateCustomModel(codeWriter);
                    File.WriteAllText(destination, sw.ToString());
                    Console.WriteLine("File generated: {0}", destination);
                }
            }
        }
    }
}
