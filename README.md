CURL example

```
curl --unix-socket ~/cpp/virusim/virusim.sock http:/e/world/population
curl --unix-socket ~/cpp/virusim/virusim.sock http:/e/world/infected
curl --unix-socket ~/cpp/virusim/virusim.sock http:/e/<country_name>/population
curl --unix-socket ~/cpp/virusim/virusim.sock http:/e/<country_name>/population_density
curl --unix-socket ~/cpp/virusim/virusim.sock http:/e/<country_name>/infected

curl -XPOST --unix-socket ~/cpp/virusim/virusim.sock http:/e/system/off
curl -XPOST --unix-socket ~/cpp/virusim/virusim.sock http:/e/system/reset
```
