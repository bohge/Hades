__include class

class Animal is
	-- _init is a special constructor method, invoked by class.new
	function _init(self,name)
		self.name = name
	end

	-- you don't have to qualify method names, since our environment
	-- is the class metatable!
    function speak(self)
        print ('growl '..self:get_name())
    end

    function get_name(self)
        return self.name
    end
end

class Dog(Animal) is

	-- necessary to explicitly call the base class ctor
	function _init(self,name)
		_base._init(self,name)
	end

	function speak(self)
		G.print('bark '..self:get_name())
	end
end

class Cat(Animal) is
	local print = G.print

	-- in simple cases, we can just use the base class ctor
	_init = _base._init

	function speak(self)
		print('meow '..self:get_name())
	end

	function get_name(self)
		return (self.name.. ' rrorrra!')
	end
end

a = Dog.new('fido')
a:speak()
c = Cat.new('felix')
c:speak()





