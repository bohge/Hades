--Character  角色系列的基类，会继承出 PlayerOther,PlayerMySelf, PlayerNPC等
local ObjectDynamic = require "object.ObjectDynamic"
local ObjectCharacter = {
	Name,
	
	MoveSpeed,
	WalkSpeed,
	HpMax,	
	HpRecover, --生命值回复速度
	HP,
	CampId,
	IsMove,
	MoveDir,

	WeponID,
	FaceYaw,
	FacePitch,
	AI,

};
CHARACTER_TYPE={
	 CT_INVALID	= -1,
     CT_PLAYERMYSELF = 0,-- 玩家自己
     CT_PLAYEROTHER = 1,-- 其他玩家
     CT_MONSTER = 2 ,-- 怪物/NPC
}


setmetatable(ObjectCharacter, ObjectDynamic)

ObjectCharacter.__index = ObjectCharacter

function ObjectCharacter:new(o)
	o = ObjectDynamic:new(o);			
	setmetatable(o,ObjectCharacter);	
	o.Name = "ObjectCharacter";
	return o;	
end
function ObjectCharacter:Cast(o)	
	o = ObjectDynamic:Cast(o);
	setmetatable(o,ObjectCharacter);			
	return o;	
end


function ObjectCharacter:Tick( DeltaTime )
	print(self.Name  .. ": Tick " )
	-- body
	--AI TICK
	--self.AI.Tick();
end


--AI相关
function ObjectCharacter:InitAI( ... )
	-- body
end

function ObjectCharacter:IsNPC( ... )
	return false;
end
function ObjectCharacter:SetName( name)
	self.Name = name;
end
function ObjectCharacter:GetName(  )
	return self.Name;
end
function ObjectCharacter:SetHP( hp )
	self.HP = hp;
end
function ObjectCharacter:GetHP(  )
	return self.HP;
end
--Common
function ObjectCharacter:IsDie( )
	return false;
end


function ObjectCharacter:IsMoving( ... )
	-- body
end

function ObjectCharacter:Initial( ... )
	-- body
end

function ObjectCharacter:GetCharacterType( ... )
	return CHARACTER_TYPE.CT_INVALID;
end
function ObjectCharacter:ChangeWeapon( NewWeaponId )
	self.WeponID = NewWeaponId;
end
function ObjectCharacter:SetFaceDir( yaw,pitch )
	self.FaceYaw = yaw;
	self.FacePitch = pitch;
end
function ObjectCharacter:Fire( ... )
	-- body
end
function ObjectCharacter:HandPositionRotation( lefthandpos,lefthandrotation,righthandpos,righthandrotation,headpos,headrotation )
	-- body
end
function ObjectCharacter:MoveMeet( posx,posy,posz )
	-- body
end
function ObjectCharacter:StartMove(dir,posx,posy,posz)
	
end
function ObjectCharacter:EndMove(posx,posy,posz)
	--posx,posy.posz
end
function ObjectCharacter:WeaponReload()
	
end
function ObjectCharacter:Revive( ... )
	-- body
end
function ObjectCharacter:ProjectileDamage( ... )
	-- body
end
function ObjectCharacter:BeHit(weapontype,islefthand,hitplayerid,damage)
	--self:HP = self:HP - damage;		

	--if(self:HP < 0) then
	--	--广播死亡
	--end
end






function ObjectCharacter:GetCharacterData( ... )
	-- body
end
function ObjectCharacter:GetConstCharacterData( ... )
	-- body
end
function ObjectCharacter:GetWeaponName( ... )
	-- body
end
function ObjectCharacter:Jump( ... )
	-- body
end
function ObjectCharacter:SitDown( ... )
	-- body
end
function ObjectCharacter:StandUp( ... )
	-- body
end
function ObjectCharacter:GetAI( ... )
	-- body
end
function ObjectCharacter:IsIdle( ... )
	-- body
end

function ObjectCharacter:CanMove( ... )
	-- body
end

return ObjectCharacter;