-- lib/ is not in default Lua package paths
package.path = './lib/?.lua;' .. package.path;
package.cpath = './lib/?.so;' .. package.cpath;
