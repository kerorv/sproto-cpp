#!/usr/bin/lua
local parser = require "sprotoparser"

local spfile = arg[1]
file = io.open(spfile, "r")
scheme = file:read "*a"
file:close()
file = nil

local pbfile = arg[2]
local bin = parser.parse(scheme)
file = io.open(pbfile, "ab")
file:write(bin)
file:close()

