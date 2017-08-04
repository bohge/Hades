
namespace CapnProtoStruct
{
    [global::CapnProto.StructAttribute(global::CapnProto.ElementSize.InlineComposite, 0, 1)]
    [global::CapnProto.IdAttribute(0xc6d18ba3b1994903)]
    public partial struct AddressBook : global::CapnProto.IPointer
    {
        private global::CapnProto.Pointer m_;
        private AddressBook(global::CapnProto.Pointer pointer){ this.m_ = pointer; }
        public static explicit operator global::CapnProtoStruct.AddressBook(global::CapnProto.Pointer pointer) { return new global::CapnProtoStruct.AddressBook(pointer); }
        public static implicit operator global::CapnProto.Pointer (global::CapnProtoStruct.AddressBook obj) { return obj.m_; }
        public override int GetHashCode() { return this.m_.GetHashCode(); }
        partial void OnToString(ref string s);
        public override string ToString() { string s = null; this.OnToString(ref s); return s ?? this.m_.ToString(); }
        public override bool Equals(object obj) { return obj is global::CapnProtoStruct.AddressBook && (this.m_ == ((global::CapnProtoStruct.AddressBook)obj).m_); }
        global::CapnProto.Pointer global::CapnProto.IPointer.Pointer { get { return this.m_; } }
        public global::CapnProtoStruct.AddressBook Dereference() { return (global::CapnProtoStruct.AddressBook)this.m_.Dereference(); }
        public static global::CapnProtoStruct.AddressBook Create(global::CapnProto.Pointer parent) { return (global::CapnProtoStruct.AddressBook)parent.Allocate(0, 1); }
        [global::CapnProto.FieldAttribute(0, pointer: 0)]
        public global::CapnProto.FixedSizeList<global::CapnProtoStruct.Person> people
        {
            get
            {
                return (global::CapnProto.FixedSizeList<global::CapnProtoStruct.Person>)this.m_.GetPointer(0);
            }
            set
            {
                this.m_.SetPointer(0, value);
            }
        }
    }
    [global::CapnProto.StructAttribute(global::CapnProto.ElementSize.InlineComposite, 1, 4)]
    [global::CapnProto.IdAttribute(0xe192b45d48e7f6a4)]
    public partial struct Person : global::CapnProto.IPointer
    {
        private global::CapnProto.Pointer m_;
        private Person(global::CapnProto.Pointer pointer){ this.m_ = pointer; }
        public static explicit operator global::CapnProtoStruct.Person(global::CapnProto.Pointer pointer) { return new global::CapnProtoStruct.Person(pointer); }
        public static implicit operator global::CapnProto.Pointer (global::CapnProtoStruct.Person obj) { return obj.m_; }
        public override int GetHashCode() { return this.m_.GetHashCode(); }
        partial void OnToString(ref string s);
        public override string ToString() { string s = null; this.OnToString(ref s); return s ?? this.m_.ToString(); }
        public override bool Equals(object obj) { return obj is global::CapnProtoStruct.Person && (this.m_ == ((global::CapnProtoStruct.Person)obj).m_); }
        global::CapnProto.Pointer global::CapnProto.IPointer.Pointer { get { return this.m_; } }
        public global::CapnProtoStruct.Person Dereference() { return (global::CapnProtoStruct.Person)this.m_.Dereference(); }
        public static global::CapnProtoStruct.Person Create(global::CapnProto.Pointer parent) { return (global::CapnProtoStruct.Person)parent.Allocate(1, 4); }
        [global::CapnProto.FieldAttribute(0, 0, 32)]
        public uint id
        {
            get
            {
                return this.m_.GetUInt32(0);
            }
            set
            {
                this.m_.SetUInt32(0, value);
            }
        }
        [global::CapnProto.FieldAttribute(1, pointer: 0)]
        public global::CapnProto.Text name
        {
            get
            {
                return (global::CapnProto.Text)this.m_.GetPointer(0);
            }
            set
            {
                this.m_.SetPointer(0, value);
            }
        }
        [global::CapnProto.FieldAttribute(2, pointer: 1)]
        public global::CapnProto.Text email
        {
            get
            {
                return (global::CapnProto.Text)this.m_.GetPointer(1);
            }
            set
            {
                this.m_.SetPointer(1, value);
            }
        }
        [global::CapnProto.FieldAttribute(3, pointer: 2)]
        public global::CapnProto.FixedSizeList<global::CapnProtoStruct.Person.PhoneNumber> phones
        {
            get
            {
                return (global::CapnProto.FixedSizeList<global::CapnProtoStruct.Person.PhoneNumber>)this.m_.GetPointer(2);
            }
            set
            {
                this.m_.SetPointer(2, value);
            }
        }
        public global::CapnProtoStruct.Person.employmentGroup employment
        {
            get
            {
                return new global::CapnProtoStruct.Person.employmentGroup(this.m_);
            }
        }
        [global::CapnProto.Group, global::CapnProto.Id(0xd4fc12f36047cec3)]
        public partial struct employmentGroup
        {
            private global::CapnProto.Pointer m_;
            internal employmentGroup(global::CapnProto.Pointer pointer)
            {
                this.m_ = pointer;
            }
            [global::CapnProto.FieldAttribute(5, pointer: 3)]
            public global::CapnProto.Text employer
            {
                get
                {
                    return (global::CapnProto.Text)this.m_.GetPointer(this.m_.GetUInt16(2) == (ushort)1 ? 3 : -1);
                }
                set
                {
                    this.m_.SetPointer(this.m_.GetUInt16(2) == (ushort)1 ? 3 : -1, value);
                }
            }
            [global::CapnProto.FieldAttribute(6, pointer: 3)]
            public global::CapnProto.Text school
            {
                get
                {
                    return (global::CapnProto.Text)this.m_.GetPointer(this.m_.GetUInt16(2) == (ushort)2 ? 3 : -1);
                }
                set
                {
                    this.m_.SetPointer(this.m_.GetUInt16(2) == (ushort)2 ? 3 : -1, value);
                }
            }
            public enum Unions
            {
                unemployed = 0,
                employer = 1,
                school = 2,
                selfEmployed = 3,
            }
            public global::CapnProtoStruct.Person.employmentGroup.Unions Union
            {
                get
                {
                    return (global::CapnProtoStruct.Person.employmentGroup.Unions)this.m_.GetUInt16(2);
                }
                set
                {
                    this.m_.SetUInt16(2, (ushort)value);
                }
            }
        }
        [global::CapnProto.StructAttribute(global::CapnProto.ElementSize.InlineComposite, 1, 1)]
        [global::CapnProto.IdAttribute(0xb45229fbdf9df247)]
        public partial struct PhoneNumber : global::CapnProto.IPointer
        {
            private global::CapnProto.Pointer m_;
            private PhoneNumber(global::CapnProto.Pointer pointer){ this.m_ = pointer; }
            public static explicit operator global::CapnProtoStruct.Person.PhoneNumber(global::CapnProto.Pointer pointer) { return new global::CapnProtoStruct.Person.PhoneNumber(pointer); }
            public static implicit operator global::CapnProto.Pointer (global::CapnProtoStruct.Person.PhoneNumber obj) { return obj.m_; }
            public override int GetHashCode() { return this.m_.GetHashCode(); }
            partial void OnToString(ref string s);
            public override string ToString() { string s = null; this.OnToString(ref s); return s ?? this.m_.ToString(); }
            public override bool Equals(object obj) { return obj is global::CapnProtoStruct.Person.PhoneNumber && (this.m_ == ((global::CapnProtoStruct.Person.PhoneNumber)obj).m_); }
            global::CapnProto.Pointer global::CapnProto.IPointer.Pointer { get { return this.m_; } }
            public global::CapnProtoStruct.Person.PhoneNumber Dereference() { return (global::CapnProtoStruct.Person.PhoneNumber)this.m_.Dereference(); }
            public static global::CapnProtoStruct.Person.PhoneNumber Create(global::CapnProto.Pointer parent) { return (global::CapnProtoStruct.Person.PhoneNumber)parent.Allocate(1, 1); }
            [global::CapnProto.FieldAttribute(0, pointer: 0)]
            public global::CapnProto.Text number
            {
                get
                {
                    return (global::CapnProto.Text)this.m_.GetPointer(0);
                }
                set
                {
                    this.m_.SetPointer(0, value);
                }
            }
            [global::CapnProto.FieldAttribute(1, 0, 16)]
            public global::CapnProtoStruct.Person.PhoneNumber.Type type
            {
                get
                {
                    return (global::CapnProtoStruct.Person.PhoneNumber.Type)this.m_.GetUInt16(0);
                }
                set
                {
                    this.m_.SetUInt16(0, (ushort)value);
                }
            }
            public enum Type
            {
                mobile = 0,
                home = 1,
                work = 2,
            }
        }
    }
}