-- This macro 'unrolls' a table, efficiently injecting it into the module namespace.
-- e.g. 'using (math)' is the same as
-- local log = math.log
-- local max = math.max
-- ....
macro.define('using',{'tbl'},
    function(ls,n)
        local tbl = _G[n[2]]
        local lset = macro.lex 'local f = T.f;'
        local subst,put = macro.subst_putter()
        for k,v in pairs(tbl) do
            put(macro.replace({'f','T'},{macro.name(k),n},lset))
        end
        return subst
    end)
