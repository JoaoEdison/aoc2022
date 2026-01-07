#!/bin/bash


echo "01"
cd 01-calorie-counting/
rm calorie_counting.out
cc calorie_counting.c -o calorie_counting.out 2> /dev/null
./calorie_counting.out 1 < input | diff - 01_c.sol
./calorie_counting.out 2 < input | diff - 02_c.sol
cd ..

echo "02"
cd 02-rock-paper-scissors/
rm rock_paper_scissors.out
cc rock_paper_scissors.c -o rock_paper_scissors.out 2> /dev/null
./rock_paper_scissors.out 1 < input | diff - 01_c.sol
./rock_paper_scissors.out 2 < input | diff - 02_c.sol
cd ..

echo "03"
cd 03-rucksack-reorganization/
rm rucksack_reorganization.out
cc rucksack_reorganization.c -o rucksack_reorganization.out 2> /dev/null
./rucksack_reorganization.out 1 < input | diff - 01_c.sol
./rucksack_reorganization.out 2 < input | diff - 02_c.sol
cd ..

echo "04"
cd 04-camp-cleanup/
rm camp_cleanup.out
cc camp_cleanup.c -o camp_cleanup.out 2> /dev/null
./camp_cleanup.out 1 < input | diff - 01_c.sol
./camp_cleanup.out 2 < input | diff - 02_c.sol
cd ..

echo "05"
cd 05-supply-stacks/
rm supply_stacks.out
cc supply_stacks.c -o supply_stacks.out 2> /dev/null
./supply_stacks.out 1 < input | diff - 01_c.sol
./supply_stacks.out 2 < input | diff - 02_c.sol
cd ..

echo "06"
cd 06-tuning-trouble/
rm tuning_trouble.out
cc tuning_trouble.c -o tuning_trouble.out 2> /dev/null
./tuning_trouble.out 1 < input | diff - 01_c.sol
./tuning_trouble.out 2 < input | diff - 02_c.sol
cd ..

echo "07"
cd 07-no-space-left-on-device/
rm no_space_left_on_device.out
cc no_space_left_on_device.c -o no_space_left_on_device.out 2> /dev/null
./no_space_left_on_device.out < input | diff - both_c.sol
cd ..

echo "08"
cd 08-treetop-tree-house/
rm treetop_tree_house.out
cc treetop_tree_house.c -o treetop_tree_house.out 2> /dev/null
./treetop_tree_house.out < input | diff - both_c.sol
cd ..

echo "09"
cd 09-rope-bridge/
rm rope_bridge.out
cc rope_bridge.c -o rope_bridge.out 2> /dev/null
./rope_bridge.out 1 < input | diff - 01_c.sol
./rope_bridge.out 2 < input | diff - 02_c.sol
cd ..

echo "10"
cd 10-cathode-ray-tube/
rm cathode_ray_tube1.out cathode_ray_tube2.out
cc cathode_ray_tube1.c -o cathode_ray_tube1.out 2> /dev/null
cc cathode_ray_tube2.c -o cathode_ray_tube2.out 2> /dev/null
./cathode_ray_tube1.out < input | diff - 01_c.sol
./cathode_ray_tube2.out < input | diff - 02_c.sol
cd ..

echo "11"
cd 11-monkey-in-the-middle/
rm monkey_in_the_middle.out
cc monkey_in_the_middle.c -o monkey_in_the_middle.out 2> /dev/null
./monkey_in_the_middle.out 1 < input | diff - 01_c.sol
./monkey_in_the_middle.out 2 < input | diff - 02_c.sol
cd ..

echo "12"
cd 12-hill-climbing-algorithm/
rm hill_climbing.out
cc hill_climbing.c -o hill_climbing.out 2> /dev/null
./hill_climbing.out 1 < input | diff - 01_c.sol
./hill_climbing.out 2 < input | diff - 02_c.sol
cd ..

echo "13"
cd 13-distress-signal/
rm distress_signal.out
cc distress_signal.c -o distress_signal.out 2> /dev/null
./distress_signal.out 1 < input | diff - 01_c.sol
./distress_signal.out 2 < input | diff - 02_c.sol
cd ..

echo "14"
cd 14-regolith-reservoir/
rm regolith_reservoir.out
cc regolith_reservoir.c -o regolith_reservoir.out 2> /dev/null
./regolith_reservoir.out 1 < input | diff - 01_c.sol
./regolith_reservoir.out 2 < input | diff - 02_c.sol
cd ..

echo "15"
cd 15-beacon-exclusion-zone/
rm beacon_exclusion_zone.out
cc beacon_exclusion_zone.c -o beacon_exclusion_zone.out 2> /dev/null
./beacon_exclusion_zone.out 1 2000000 < input | diff - 01_c.sol
./beacon_exclusion_zone.out 2 4000000 < input | diff - 02_c.sol
cd ..

echo "16"
cd 16-proboscidea-volcanium/
rm proboscidea_volcanium.out
cc proboscidea_volcanium.c minheap.c combinations.c -o proboscidea_volcanium.out 2> /dev/null
./proboscidea_volcanium.out 1 < input | diff - 01_c.sol
./proboscidea_volcanium.out 2 < input | diff - 02_c.sol
cd ..

echo "17"
cd 17-pyroclastic-flow/
rm pyroclastic_flow.out
cc pyroclastic_flow.c -o pyroclastic_flow.out 2> /dev/null
./pyroclastic_flow.out 1 < input | diff - 01_c.sol
./pyroclastic_flow.out 2 < input | diff - 02_c.sol
cd ..

echo "18"
cd 18-boiling-boulders/
rm boiling_boulders.out
cc boiling_boulders.c -o boiling_boulders.out 2> /dev/null
./boiling_boulders.out 1 < input | diff - 01_c.sol
./boiling_boulders.out 2 < input | diff - 02_c.sol
cd ..

echo "19"
cd 19-not-enough-minerals/
rm not_enough_minerals.out
cc not_enough_minerals.c -o not_enough_minerals.out 2> /dev/null
./not_enough_minerals.out 1 < input | diff - 01_c.sol
./not_enough_minerals.out 2 < input | diff - 02_c.sol
cd ..

echo "20"
cd 20-grove-positioning-system/
rm grove_positioning_system.out
cc grove_positioning_system.c -o grove_positioning_system.out 2> /dev/null
./grove_positioning_system.out 1 < input | diff - 01_c.sol
./grove_positioning_system.out 2 < input | diff - 02_c.sol
cd ..

echo "21"
cd 21-monkey-math/
rm monkey_math.out
cc monkey_math.c -lm -o monkey_math.out 2> /dev/null
./monkey_math.out 1 < input | diff - 01_c.sol
./monkey_math.out 2 < input | diff - 02_c.sol
cd ..
