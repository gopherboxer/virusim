| Method | URL |
| --- | --- |
| POST | /system/off |
| POST | /system/reset |

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

Simulator Algorithm
https://en.wikipedia.org/wiki/Compartmental_models_in_epidemiology#The_SIR_model
