--
-- Created by IntelliJ IDEA.
-- User: juston.davies
-- Date: 1/28/16
-- Time: 4:42 PM
-- To change this template use File | Settings | File Templates.
--
package.cpath = './out/?.so;' .. package.cpath

local nexus_handle = require("nexus_handle")

local temp = io.popen("/sbin/ifconfig|grep 'Ethernet '|awk '{print $5}'");
local mac = temp:read("*l");
temp:close();

env = {}
env.services = {
    store = {bonsai_update = "tcp://192.168.1.159:5056"},
    services_uri = "tcp://192.168.1.159",
    identity = mac:gsub(":","-"):lower()
}

store = {}

for k,v in pairs(nexus_handle) do print(k,v) end
nexus_handle.test()

print("Wowowowowowowowow")