#!/bin/bash
redis-cli -p 13000 -a e2345 shutdown
redis-server redis_1.conf

redis-cli -p 13001 -a e2345 shutdown
redis-server redis_2.conf

