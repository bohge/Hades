using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CapnProto;
using CapnProtoStruct;
using System.IO;


namespace capnp_test
{
    class Program
    {
        static void Main(string[] args)
        {
            byte[] m_Buffer;
            MemoryStream m_Stream;
            m_Buffer = new byte[4096];
            m_Stream = new MemoryStream(4096);
            {
                var msg = Message.Load(m_Buffer);
                AddressBook ab = AddressBook.Create(msg);
                msg.Root = ab;
                ab.people = FixedSizeList<global::CapnProtoStruct.Person>.Create(msg, 1);
                Person p = ab.people[0];//目前已知只能通过这种方式修改list，不能反向赋值

                p.id = 123;
                p.name = Text.Create(msg, "Alice");
                p.email = Text.Create(msg, "alice@example.com");
                p.phones = FixedSizeList<global::CapnProtoStruct.Person.PhoneNumber>.Create(msg, 1);
                Person.PhoneNumber pn1 = p.phones[0];
                pn1.number = Text.Create(msg, "555-1212");
                pn1.type = Person.PhoneNumber.Type.mobile;

                Person.employmentGroup eg = p.employment;
                eg.Union = Person.employmentGroup.Unions.school;
                eg.school = Text.Create(msg, "MIT");                
                msg.Write(m_Stream);                
            }
            {
                m_Stream.Seek(0, SeekOrigin.Begin);
                using (var msg = Message.Load(m_Stream))
                {
                    while (msg.ReadNext())
                    {
                        AddressBook ab = (AddressBook)msg.Root;
                        Person p = ab.people[0];
                        Console.WriteLine(p.name.ToString());
                        Console.WriteLine(p.id.ToString());
                        Console.WriteLine(p.email.ToString());
                        Person.PhoneNumber pn1 = p.phones[0];
                        Console.WriteLine(pn1.number.ToString());
                        Console.WriteLine(pn1.type.ToString());
                        Person.employmentGroup eg = p.employment;
                        Console.WriteLine(eg.Union.ToString());
                        Console.WriteLine(eg.school.ToString());
                    }
                }
            }
        }
    }
}
