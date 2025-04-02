
// This file contains all the Minecraft block -> Minetest node mappings.

// To see a list of block IDs in Minecraft 1.12: https://minecraft-ids.grahamedgecombe.com/

// ## HELPER MACROS:

// Main helper macro, gets wrapped by all other macros below
#define CONV_ALL(id, mcn, datas, mtn, p2, is_tool, cb) \
	static_assert(id <= MC_ID_MAX, "MC ID is out of range! (update MC_ID_MAX if adding new items)"); \
	add_conversion(id, mcn, datas, mtn, p2, is_tool, cb)

// Convert a basic block without any specific datavalue
#define CONV(id, mcn, mtn) CONV_D(id, mcn, nullptr, mtn)

// Convert a block with a specific datavalue
#define CONV_D(id, mcn, datas, mtn) CONV_DP(id, mcn, datas, mtn, 0)

// Convert a basic block into a specific minetest param2 value
#define CONV_P(id, mcn, mtn, p2); CONV_DP(id, mcn, nullptr, mtn, p2)

// Convert a block with a specific datavalue to a specific param2 value
#define CONV_DP(id, mcn, datas, mtn, p2) CONV_DP_CB(id, mcn, datas, mtn, p2, nullptr)

// CONV_DP with a conversion callback as the last argument
#define CONV_DP_CB(id, mcn, datas, mtn, p2, cb) CONV_ALL(id, mcn, datas, mtn, p2, false, cb)

// tool
#define CONV_TOOL(id, mcn, mtn) CONV_ALL(id, mcn, nullptr, mtn, 0, true, nullptr)

// Convert a stair block, mapping datavalue to param2 facedir
#define CONV_STAIR(id, mcn, mtn) \
	CONV_DP_CB(id, mcn, "0", mtn, 1, update_node_light); \
	CONV_DP_CB(id, mcn, "1", mtn, 3, update_node_light); \
	CONV_DP_CB(id, mcn, "2", mtn, 2, update_node_light); \
	CONV_DP_CB(id, mcn, "3", mtn, 0, update_node_light); \
	CONV_DP_CB(id, mcn, "4", mtn, 23, update_node_light); \
	CONV_DP_CB(id, mcn, "5", mtn, 21, update_node_light); \
	CONV_DP_CB(id, mcn, "6", mtn, 22, update_node_light); \
	CONV_DP_CB(id, mcn, "7", mtn, 20, update_node_light)

// Convert a slab block, mapping up-down datavalue to respective mineclonia node
#define CONV_SLAB(id, mcn, dbottom, dtop, mtn) \
	CONV_DP_CB(id, mcn, #dbottom, mtn, 0, update_node_light); \
	CONV_DP_CB(id, mcn, #dtop, mtn "_top", 0, update_node_light)

// Convert a trapdoor into minetest
#define CONV_TRAPDOOR(id, mcn, mtn) \
	CONV_DP(id, mcn, "0", mtn, 2); \
	CONV_DP(id, mcn, "1", mtn, 0); \
	CONV_DP(id, mcn, "2", mtn, 1); \
	CONV_DP(id, mcn, "3", mtn, 3); \
	CONV_DP(id, mcn, "4", mtn "_open", 2); \
	CONV_DP(id, mcn, "5", mtn "_open", 0); \
	CONV_DP(id, mcn, "6", mtn "_open", 1); \
	CONV_DP(id, mcn, "7", mtn "_open", 3); \
	CONV_DP(id, mcn, "8", mtn, 22); \
	CONV_DP(id, mcn, "9", mtn, 20); \
	CONV_DP(id, mcn, "10", mtn, 23); \
	CONV_DP(id, mcn, "11", mtn, 21); \
	CONV_DP(id, mcn, "12", mtn "_open", 22); \
	CONV_DP(id, mcn, "13", mtn "_open", 20); \
	CONV_DP(id, mcn, "14", mtn "_open", 23); \
	CONV_DP(id, mcn, "15", mtn "_open", 21)

// Convert rotateable tree log
#define CONV_LOG(id, mcn, d, mtn) \
	CONV_DP(id, mcn, #d,                                   mtn, 0); \
	CONV_DP(id, mcn, std::to_string(d + (1 << 2)).c_str(), mtn, 12); \
	CONV_DP(id, mcn, std::to_string(d + (2 << 2)).c_str(), mtn, 4); \
	CONV_DP(id, mcn, std::to_string(d + (3 << 2)).c_str(), mtn, 0)

// Convert wallmounted block (e.g. torches) variants to minetest's counterparts
// (I don't think this should be used?)
#define CONV_WALLMOUNTED(id, mcn, mtn, dt, db, dn, de, ds, dw) \
	CONV_DP(id, mcn, #dt, mtn, 0); \
	CONV_DP(id, mcn, #db, mtn, 1); \
	CONV_DP(id, mcn, #dn, mtn, 5); \
	CONV_DP(id, mcn, #de, mtn, 3); \
	CONV_DP(id, mcn, #ds, mtn, 4); \
	CONV_DP(id, mcn, #dw, mtn, 2)

// Fence gates
// TODO: Check orientation 0:N 3:W 2:S 1:E
#define CONV_GATE(id, mcn, mtn) \
	CONV_DP(id, mcn, "0", mtn "", 0); \
	CONV_DP(id, mcn, "1", mtn "", 3); \
	CONV_DP(id, mcn, "2", mtn "", 2); \
	CONV_DP(id, mcn, "3", mtn "", 1); \
	CONV_DP(id, mcn, "4", mtn "_open", 0); \
	CONV_DP(id, mcn, "5", mtn "_open", 2); \
	CONV_DP(id, mcn, "6", mtn "_open", 3); \
	CONV_DP(id, mcn, "7", mtn "_open", 1)

// TODO: Door orientations and states need fixing
#if MC_VERSION < 102
#define CONV_DOOR(id, mcn, mtn) \
	CONV_DP_CB(id, mcn, "0", mtn "_b_2", 0, old_door_set_right); \
	CONV_DP_CB(id, mcn, "1", mtn "_b_2", 1, old_door_set_right); \
	CONV_DP_CB(id, mcn, "2", mtn "_b_2", 2, old_door_set_right); \
	CONV_DP_CB(id, mcn, "3", mtn "_b_2", 3, old_door_set_right); \
	CONV_DP(id, mcn, "4", mtn "_b_1", 3); \
	CONV_DP(id, mcn, "5", mtn "_b_1", 0); \
	CONV_DP(id, mcn, "6", mtn "_b_1", 1); \
	CONV_DP(id, mcn, "7", mtn "_b_1", 2); \
	CONV_DP_CB(id, mcn, "8", mtn "_t_2", 0, old_door_set_right); \
	CONV_DP_CB(id, mcn, "9", mtn "_t_2", 1, old_door_set_right); \
	CONV_DP_CB(id, mcn, "10", mtn "_t_2", 2, old_door_set_right); \
	CONV_DP_CB(id, mcn, "11", mtn "_t_2", 3, old_door_set_right); \
	CONV_DP(id, mcn, "12", mtn "_t_1", 3); \
	CONV_DP(id, mcn, "13", mtn "_t_1", 0); \
	CONV_DP(id, mcn, "14", mtn "_t_1", 1); \
	CONV_DP(id, mcn, "15", mtn "_t_1", 2);
#else // else if MC_VERSION >= 102
#define CONV_DOOR(id, mcn, mtn) \
	CONV_DP_CB(id, mcn, "0", mtn "_b_1", 1, finish_door); \
	CONV_DP_CB(id, mcn, "1", mtn "_b_1", 2, finish_door); \
	CONV_DP_CB(id, mcn, "2", mtn "_b_1", 3, finish_door); \
	CONV_DP_CB(id, mcn, "3", mtn "_b_1", 0, finish_door); \
	CONV_DP_CB(id, mcn, "4", mtn "_b_2", 5, finish_door); \
	CONV_DP_CB(id, mcn, "5", mtn "_b_2", 6, finish_door); \
	CONV_DP_CB(id, mcn, "6", mtn "_b_2", 7, finish_door); \
	CONV_DP_CB(id, mcn, "7", mtn "_b_2", 4, finish_door); \
	CONV_D(id, mcn, "8,9,10,11,12,13,14,15", mtn "_t_1");
#endif // MC_VERSION < 102



// ## MAPPINGS START HERE:

CONV(0, "minecraft:air", "air");

CONV_D(1, "minecraft:stone", "0", "mcl_core:stone");
CONV_D(1, "minecraft:stone", "1", "mcl_core:granite");
CONV_D(1, "minecraft:stone", "2", "mcl_core:granite_smooth");
CONV_D(1, "minecraft:stone", "3", "mcl_core:diorite");
CONV_D(1, "minecraft:stone", "4", "mcl_core:diorite_smooth");
CONV_D(1, "minecraft:stone", "5", "mcl_core:andesite");
CONV_D(1, "minecraft:stone", "6", "mcl_core:andesite_smooth");

CONV(2, "minecraft:grass", "mcl_core:dirt_with_grass");

CONV_D(3, "minecraft:dirt", "0", "mcl_core:dirt");
CONV_D(3, "minecraft:dirt", "1", "mcl_core:coarse_dirt");
CONV_D(3, "minecraft:dirt", "2", "mcl_core:podzol");

CONV(4, "minecraft:cobble", "mcl_core:cobble");

CONV_D(5, "minecraft:planks", "0", "mcl_trees:wood_oak");
CONV_D(5, "minecraft:planks", "1", "mcl_trees:wood_spruce");
CONV_D(5, "minecraft:planks", "2", "mcl_trees:wood_birch");
CONV_D(5, "minecraft:planks", "3", "mcl_trees:wood_jungle");
CONV_D(5, "minecraft:planks", "4", "mcl_trees:wood_acacia");
CONV_D(5, "minecraft:planks", "5", "mcl_trees:wood_dark_oak");

// Saplings have a "ready to grow" flag, which we ignore.
CONV_D(6, "minecraft:sapling", "0,8", "mcl_trees:sapling_oak");
CONV_D(6, "minecraft:sapling", "1,9", "mcl_trees:sapling_spruce");
CONV_D(6, "minecraft:sapling", "2,10", "mcl_trees:sapling_birch");
CONV_D(6, "minecraft:sapling", "3,11", "mcl_trees:sapling_jungle");
CONV_D(6, "minecraft:sapling", "4,12", "mcl_trees:sapling_acacia");
CONV_D(6, "minecraft:sapling", "5,13", "mcl_trees:sapling_dark_oak");

CONV(7, "minecraft:bedrock", "mcl_core:bedrock");

CONV(8, "minecraft:flowing_water", "mcl_core:water_flowing");
CONV(9, "minecraft:water", "mcl_core:water_source");
CONV(10, "minecraft:flowing_lava", "mcl_core:lava_flowing");
CONV(11, "minecraft:lava", "mcl_core:lava_source");

CONV_D(12, "minecraft:sand", "0", "mcl_core:sand");
CONV_D(12, "minecraft:sand", "1", "mcl_core:redsand");

CONV(13, "minecraft:gravel", "mcl_core:gravel");

CONV(14, "minecraft:gold_ore", "mcl_core:stone_with_gold");
CONV(15, "minecraft:iron_ore", "mcl_core:stone_with_iron");
CONV(16, "minecraft:coal_ore", "mcl_core:stone_with_coal");

CONV_LOG(17, "minecraft:log", 0, "mcl_trees:tree_oak");
CONV_LOG(17, "minecraft:log", 1, "mcl_trees:tree_spruce");
CONV_LOG(17, "minecraft:log", 2, "mcl_trees:tree_birch");
CONV_LOG(17, "minecraft:log", 3, "mcl_trees:tree_jungle");

CONV_D(18, "minecraft:leaves", "0,8", "mcl_trees:leaves_oak");
CONV_D(18, "minecraft:leaves", "1,9", "mcl_trees:leaves_spruce");
CONV_D(18, "minecraft:leaves", "2,10", "mcl_trees:leaves_birch");
CONV_D(18, "minecraft:leaves", "3,11", "mcl_trees:leaves_jungle");
CONV_DP(18, "minecraft:leaves", "4,12", "mcl_trees:leaves_oak", 1);
CONV_DP(18, "minecraft:leaves", "5,13", "mcl_trees:leaves_spruce", 1);
CONV_DP(18, "minecraft:leaves", "6,14", "mcl_trees:leaves_birch", 1);
CONV_DP(18, "minecraft:leaves", "7,15", "mcl_trees:leaves_jungle", 1);

CONV_D(19, "minecraft:sponge", "0", "mcl_sponges:sponge");
CONV_D(19, "minecraft:sponge", "1", "mcl_sponges:sponge_wet");

CONV(20, "minecraft:glass", "mcl_core:glass");

CONV(21, "minecraft:lapis_ore", "mcl_core:stone_with_lapis");
CONV(22, "minecraft:lapis_block", "mcl_core:lapisblock");

// TODO: check orientation
CONV_DP(23, "minecraft:dispenser", "2", "mcl_dispensers:dropper", 0);
CONV_DP(23, "minecraft:dispenser", "3", "mcl_dispensers:dropper", 2);
CONV_DP(23, "minecraft:dispenser", "4", "mcl_dispensers:dropper", 3);
CONV_DP(23, "minecraft:dispenser", "5", "mcl_dispensers:dropper", 1);

CONV_D(24, "minecraft:sandstone", "0", "mcl_core:sandstone");
CONV_D(24, "minecraft:sandstone", "1", "mcl_core:sandstonecarved");
CONV_D(24, "minecraft:sandstone", "2", "mcl_core:sandstonesmooth");

CONV(25, "minecraft:noteblock", "mcl_noteblock:noteblock"); // TODO

CONV_DP(26, "minecraft:bed", "0,4", "mcl_beds:bed_red_bottom", 2); // S
CONV_DP(26, "minecraft:bed", "1,5", "mcl_beds:bed_red_bottom", 3); // W
CONV_DP(26, "minecraft:bed", "2,6", "mcl_beds:bed_red_bottom", 1); // N
CONV_DP(26, "minecraft:bed", "3,7", "mcl_beds:bed_red_bottom", 1); // E
CONV_DP(26, "minecraft:bed", "8,12", "mcl_beds:bed_red_top", 2);  // S
CONV_DP(26, "minecraft:bed", "9,13", "mcl_beds:bed_red_top", 3);  // W
CONV_DP(26, "minecraft:bed", "10,14", "mcl_beds:bed_red_top", 0); // N
CONV_DP(26, "minecraft:bed", "11,15", "mcl_beds:bed_red_top", 1); // E

// TODO?
CONV(27, "minecraft:golden_rail", "mcl_minecarts:golden_rail");  // Powered rail
CONV(28, "minecraft:detector_rail", "mcl_minecarts:detector_rail");

// TODO: Check up/down pistons
CONV_D(29, "minecraft:sticky_piston", "0", "mesecons_pistons:piston_down_sticky_off");
CONV_D(29, "minecraft:sticky_piston", "1", "mesecons_pistons:piston_up_sticky_off");

CONV_DP(29, "minecraft:sticky_piston", "2", "mcl_pistons:piston_sticky_off", 2);  // N
CONV_DP(29, "minecraft:sticky_piston", "3", "mcl_pistons:piston_sticky_off", 0);  // S
CONV_DP(29, "minecraft:sticky_piston", "4", "mcl_pistons:piston_sticky_off", 1);  // W
CONV_DP(29, "minecraft:sticky_piston", "5", "mcl_pistons:piston_sticky_off", 3);  // E

CONV_D(29, "minecraft:sticky_piston", "8", "mesecons_pistons:piston_down_sticky_on");
CONV_D(29, "minecraft:sticky_piston", "9", "mesecons_pistons:piston_up_sticky_on");

CONV_DP(29, "minecraft:sticky_piston", "10", "mcl_pistons:piston_sticky_on", 2);  // N
CONV_DP(29, "minecraft:sticky_piston", "11", "mcl_pistons:piston_sticky_on", 0);  // S
CONV_DP(29, "minecraft:sticky_piston", "12", "mcl_pistons:piston_sticky_on", 1);  // W
CONV_DP(29, "minecraft:sticky_piston", "13", "mcl_pistons:piston_sticky_on", 3);  // E

CONV(30, "minecraft:web", "mcl_core:cobweb");

CONV_D(31, "minecraft:tallgrass", "0", "mcl_core:deadbush");
CONV_D(31, "minecraft:tallgrass", "1", "mcl_flowers:tallgrass");
CONV_D(31, "minecraft:tallgrass", "2", "mcl_flowers:fern");

CONV(32, "minecraft:deadbush", "mcl_core:deadbush");

// TODO: Check up/down pistons
CONV_D(33, "minecraft:piston", "0", "mesecons_piston:piston_down_normal_off");
CONV_D(33, "minecraft:piston", "1", "mesecons_piston:piston_up_normal_off");
CONV_DP(33, "minecraft:piston", "2", "mesecons_piston:piston_normal_off", 2);  // N
CONV_DP(33, "minecraft:piston", "3", "mesecons_piston:piston_normal_off", 0);  // S
CONV_DP(33, "minecraft:piston", "4", "mesecons_piston:piston_normal_off", 1);  // W
CONV_DP(33, "minecraft:piston", "5", "mesecons_piston:piston_normal_off", 3);  // E
CONV_D(33, "minecraft:piston", "8", "mesecons_piston:piston_down_normal_on");
CONV_D(33, "minecraft:piston", "9", "mesecons_piston:piston_up_normal_on");
CONV_DP(33, "minecraft:piston", "10", "mesecons_piston:piston_normal_on", 2);  // N
CONV_DP(33, "minecraft:piston", "11", "mesecons_piston:piston_normal_on", 0);  // S
CONV_DP(33, "minecraft:piston", "12", "mesecons_piston:piston_normal_on", 1);  // W
CONV_DP(33, "minecraft:piston", "13", "mesecons_piston:piston_normal_on", 3);  // E
CONV_D(34, "minecraft:piston_head", "0", "mesecons_piston:piston_down_pusher_normal");
CONV_D(34, "minecraft:piston_head", "1", "mesecons_piston:piston_up_pusher_normal");
CONV_DP(34, "minecraft:piston_head", "2", "mesecons_piston:piston_pusher_normal", 2);  // N
CONV_DP(34, "minecraft:piston_head", "3", "mesecons_piston:piston_pusher_normal", 0);  // S
CONV_DP(34, "minecraft:piston_head", "4", "mesecons_piston:piston_pusher_normal", 1);  // W
CONV_DP(34, "minecraft:piston_head", "5", "mesecons_piston:piston_pusher_normal", 3);  // E
CONV_D(34, "minecraft:piston_head", "8", "mesecons_piston:piston_down_pusher_sticky");
CONV_D(34, "minecraft:piston_head", "9", "mesecons_piston:piston_up_pusher_sticky");
CONV_DP(34, "minecraft:piston_head", "10", "mesecons_piston:piston_pusher_sticky", 2);  // N
CONV_DP(34, "minecraft:piston_head", "11", "mesecons_piston:piston_pusher_sticky", 0);  // S
CONV_DP(34, "minecraft:piston_head", "12", "mesecons_piston:piston_pusher_sticky", 1);  // W
CONV_DP(34, "minecraft:piston_head", "13", "mesecons_piston:piston_pusher_sticky", 3);  // E

CONV_D(35, "minecraft:wool", "0", "mcl_wool:white");
CONV_D(35, "minecraft:wool", "1", "mcl_wool:orange");
CONV_D(35, "minecraft:wool", "2", "mcl_wool:magenta");
CONV_D(35, "minecraft:wool", "3", "mcl_wool:light_blue");
CONV_D(35, "minecraft:wool", "4", "mcl_wool:yellow");
CONV_D(35, "minecraft:wool", "5", "mcl_wool:lime");
CONV_D(35, "minecraft:wool", "6", "mcl_wool:pink");
CONV_D(35, "minecraft:wool", "7", "mcl_wool:grey");
CONV_D(35, "minecraft:wool", "8", "mcl_wool:silver");
CONV_D(35, "minecraft:wool", "9", "mcl_wool:cyan");
CONV_D(35, "minecraft:wool", "10", "mcl_wool:purple");
CONV_D(35, "minecraft:wool", "11", "mcl_wool:blue");
CONV_D(35, "minecraft:wool", "12", "mcl_wool:brown");
CONV_D(35, "minecraft:wool", "13", "mcl_wool:green");
CONV_D(35, "minecraft:wool", "14", "mcl_wool:red");
CONV_D(35, "minecraft:wool", "15", "mcl_wool:black");

// TODO: 36: Piston extension (block being moved by piston)

CONV(37, "minecraft:yellow_flower", "mcl_flowers:dandelion");
CONV_D(38, "minecraft:red_flower", "0", "mcl_flowers:poppy");
CONV_D(38, "minecraft:red_flower", "1", "mcl_flowers:blue_orchid");
CONV_D(38, "minecraft:red_flower", "2", "mcl_flowers:allium");
CONV_D(38, "minecraft:red_flower", "3", "mcl_flowers:azure_bluet");
CONV_D(38, "minecraft:red_flower", "4", "mcl_flowers:tulip_red");
CONV_D(38, "minecraft:red_flower", "5", "mcl_flowers:tulip_orange");
CONV_D(38, "minecraft:red_flower", "6", "mcl_flowers:tulip_white");
CONV_D(38, "minecraft:red_flower", "7", "mcl_flowers:tulip_pink");
CONV_D(38, "minecraft:red_flower", "8", "mcl_flowers:oxeye_daisy");

CONV(39, "minecraft:brown_mushroom", "mcl_mushrooms:mushroom_brown");
CONV(40, "minecraft:red_mushroom", "mcl_mushrooms:mushroom_red");

CONV(41, "minecraft:gold_block", "mcl_core:goldblock");
CONV(42, "minecraft:iron_block", "mcl_core:ironblock");

// Double slabs
CONV_D(43, "minecraft:double_stone_slab", "0", "mcl_stairs:slab_stone_double");
CONV_D(43, "minecraft:double_stone_slab", "1", "mcl_stairs:slab_sandstone_double");
CONV_D(43, "minecraft:double_stone_slab", "2", "mcl_stairs:slab_oak_double"); // todo
CONV_D(43, "minecraft:double_stone_slab", "3", "mcl_stairs:slab_cobble_double");
CONV_D(43, "minecraft:double_stone_slab", "4", "mcl_stairs:slab_brick_block_double");
CONV_D(43, "minecraft:double_stone_slab", "5", "mcl_stairs:slab_stonebrick_double");
CONV_D(43, "minecraft:double_stone_slab", "6", "mcl_stairs:slab_nether_brick");
CONV_D(43, "minecraft:double_stone_slab", "7", "mcl_stairs:slab_quartzblock_double");
CONV_D(43, "minecraft:double_stone_slab", "8", "mcl_core:stone_smooth");
CONV_D(43, "minecraft:double_stone_slab", "9", "mcl_core:sandstone");
CONV_D(43, "minecraft:double_stone_slab", "10", "mcl_nether:quartz_chiseled");

CONV_SLAB(44, "minecraft:stone_slab", 0, 8, "mcl_stairs:slab_stone");
CONV_SLAB(44, "minecraft:stone_slab", 1, 9, "mcl_stairs:slab_sandstone");
CONV_SLAB(44, "minecraft:stone_slab", 2, 10, "mcl_stairs:slab_oak");
CONV_SLAB(44, "minecraft:stone_slab", 3, 11, "mcl_stairs:slab_cobble");
CONV_SLAB(44, "minecraft:stone_slab", 4, 12, "mcl_stairs:slab_brick_block");
CONV_SLAB(44, "minecraft:stone_slab", 5, 13, "mcl_stairs:slab_stonebrick");
CONV_SLAB(44, "minecraft:stone_slab", 6, 14, "mcl_stairs:slab_nether_brick");
CONV_SLAB(44, "minecraft:stone_slab", 7, 15, "mcl_stairs:slab_quartzblock");

CONV(45, "minecraft:brick_block", "mcl_core:brick_block");
CONV(46, "minecraft:tnt", "mcl_tnt:tnt");
CONV(47, "minecraft:bookshelf", "mcl_books:bookshelf");
CONV(48, "minecraft:mossy_cobblestone", "mcl_core:mossycobble");
CONV(49, "minecraft:obsidian", "mcl_core:obsidian");

CONV_DP(50, "minecraft:torch", "0", "mcl_torches:torch", 1);
CONV_DP(50, "minecraft:torch", "1", "mcl_torches:torch_wall", 3);
CONV_DP(50, "minecraft:torch", "2", "mcl_torches:torch_wall", 2);
CONV_DP(50, "minecraft:torch", "3", "mcl_torches:torch_wall", 4);
CONV_DP(50, "minecraft:torch", "4", "mcl_torches:torch_wall", 5);
CONV_DP(50, "minecraft:torch", "5", "mcl_torches:torch", 1);

CONV(51, "minecraft:fire", "mcl_fire:fire");

CONV(52, "minecraft:mob_spawner", "mcl_mobspawners:spawner"); // TODO

CONV_STAIR(53, "minecraft:oak_stairs", "mcl_stairs:stair_oak");

// chest
CONV_DP(54, "minecraft:chest", "2", "mcl_chests:chest", 2);  // N
CONV_DP(54, "minecraft:chest", "3", "mcl_chests:chest", 0);  // S
CONV_DP(54, "minecraft:chest", "4", "mcl_chests:chest", 1);  // W
CONV_DP(54, "minecraft:chest", "5", "mcl_chests:chest", 3);  // E

// XXX: Connections aren't saved, just power level
CONV_D(55, "minecraft:redstone_wire", "0", "mesecons:wire_00000000_off");
CONV_D(55, "minecraft:redstone_wire", "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15", "mesecons:wire_00000000_on");

CONV(56, "minecraft:diamond_ore", "mcl_core:stone_with_diamond");
CONV(57, "minecraft:diamond_block", "mcl_core:diamondblock");

CONV(58, "minecraft:crafting_table", "mcl_crafting_table:crafting_table");

CONV_D(59, "minecraft:wheat", "0", "mcl_farming:wheat_1");
CONV_D(59, "minecraft:wheat", "1", "mcl_farming:wheat_2");
CONV_D(59, "minecraft:wheat", "2", "mcl_farming:wheat_3");
CONV_D(59, "minecraft:wheat", "3", "mcl_farming:wheat_4");
CONV_D(59, "minecraft:wheat", "4", "mcl_farming:wheat_5");
CONV_D(59, "minecraft:wheat", "5", "mcl_farming:wheat_6");
CONV_D(59, "minecraft:wheat", "6", "mcl_farming:wheat_7");
CONV_D(59, "minecraft:wheat", "7", "mcl_farming:wheat");

// Data is counter, only wet when counter reaches 7
CONV(60, "minecraft:farmland", "mcl_farming:soil");
CONV_D(60, "minecraft:farmland", "7", "mcl_farming:soil_wet");

CONV_DP(61, "minecraft:furnace", "2", "mcl_furnaces:furnace", 2);  // N
CONV_DP(61, "minecraft:furnace", "3", "mcl_furnaces:furnace", 0);  // S
CONV_DP(61, "minecraft:furnace", "4", "mcl_furnaces:furnace", 1);  // W
CONV_DP(61, "minecraft:furnace", "5", "mcl_furnaces:furnace", 3);  // E
CONV_DP(62, "minecraft:lit_furnace", "2", "mcl_furnaces:furnace_active", 2);
CONV_DP(62, "minecraft:lit_furnace", "3", "mcl_furnaces:furnace_active", 0);
CONV_DP(62, "minecraft:lit_furnace", "4", "mcl_furnaces:furnace_active", 1);
CONV_DP(62, "minecraft:lit_furnace", "5", "mcl_furnaces:furnace_active", 3);

// TODO
CONV_P(63, "minecraft:standing_sign", "mcl_signs:standing_sign", 1);

CONV_DOOR(64, "minecraft:wooden_door", "mcl_doors:door_oak");

CONV_DP(65, "minecraft:ladder", "2", "mcl_core:ladder", 5);
CONV_DP(65, "minecraft:ladder", "3", "mcl_core:ladder", 4);
CONV_DP(65, "minecraft:ladder", "4", "mcl_core:ladder", 2);
CONV_DP(65, "minecraft:ladder", "5", "mcl_core:ladder", 3);

CONV(66, "minecraft:rail", "mcl_minecarts:rail");

CONV_STAIR(67, "minecraft:stone_stairs", "mcl_stairs:stair_cobble");

CONV_DP(68, "minecraft:wall_sign", "2", "mcl_signs:wall_sign", 5); // N
CONV_DP(68, "minecraft:wall_sign", "3", "mcl_signs:wall_sign", 4); // S
CONV_DP(68, "minecraft:wall_sign", "4", "mcl_signs:wall_sign", 2); // W
CONV_DP(68, "minecraft:wall_sign", "5", "mcl_signs:wall_sign", 3); // E

// XXX: Support up/down levers
CONV_DP(69, "minecraft:lever", "1", "mcl_lever:lever_off", 3);  // W
CONV_DP(69, "minecraft:lever", "2", "mcl_lever:lever_off", 1);  // E
CONV_DP(69, "minecraft:lever", "3", "mcl_lever:lever_off", 0);  // N
CONV_DP(69, "minecraft:lever", "4,0,5,6,7", "mcl_lever:lever_off", 2);  // S, up, down

CONV_DP(69, "minecraft:lever", "9", "mcl_lever:lever_on", 3);   // W
CONV_DP(69, "minecraft:lever", "10", "mcl_lever:lever_on", 1);  // E
CONV_DP(69, "minecraft:lever", "11", "mcl_lever:lever_on", 0);  // N
CONV_DP(69, "minecraft:lever", "12,8,13,14,15", "mcl_lever:lever_on", 2);  // S, up, down

CONV(70, "minecraft:stone_pressure_plate", "mcl_pressureplates:pressure_plate_stone_off");

CONV_DOOR(71, "minecraft:iron_door", "mcl_doors:iron_door");

// TODO wood types?
CONV(72, "minecraft:wooden_pressure_plate", "mcl_pressureplates:pressure_plate_oak_off");

CONV(73, "minecraft:redstone_ore", "mcl_core:stone_with_redstone");
CONV(74, "minecraft:lit_redstone_ore", "mcl_core:stone_with_redstone_lit");

CONV_DP(75, "minecraft:unlit_redstone_torch", "0", "mcl_redstone_torch:redstone_torch_off", 1);
CONV_DP(75, "minecraft:unlit_redstone_torch", "1", "mcl_redstone_torch:redstone_torch_off_wall", 3);
CONV_DP(75, "minecraft:unlit_redstone_torch", "2", "mcl_redstone_torch:redstone_torch_off_wall", 2);
CONV_DP(75, "minecraft:unlit_redstone_torch", "3", "mcl_redstone_torch:redstone_torch_off_wall", 4);
CONV_DP(75, "minecraft:unlit_redstone_torch", "4", "mcl_redstone_torch:redstone_torch_off_wall", 5);
CONV_DP(75, "minecraft:unlit_redstone_torch", "5", "mcl_redstone_torch:redstone_torch_off", 1);

CONV_DP(75, "minecraft:lit_redstone_torch", "0", "mcl_redstone_torch:redstone_torch_on", 1);
CONV_DP(75, "minecraft:lit_redstone_torch", "1", "mcl_redstone_torch:redstone_torch_on_wall", 3);
CONV_DP(75, "minecraft:lit_redstone_torch", "2", "mcl_redstone_torch:redstone_torch_on_wall", 2);
CONV_DP(75, "minecraft:lit_redstone_torch", "3", "mcl_redstone_torch:redstone_torch_on_wall", 4);
CONV_DP(75, "minecraft:lit_redstone_torch", "4", "mcl_redstone_torch:redstone_torch_on_wall", 5);
CONV_DP(75, "minecraft:lit_redstone_torch", "5", "mcl_redstone_torch:redstone_torch_on", 1);
//CONV_WALLMOUNTED(75, "minecraft:unlit_redstone_torch", "mesecons_torch:mesecon_torch_off", 0, 5, 3, 2, 4, 1);
//CONV_WALLMOUNTED(76, "minecraft:lit_redstone_torch", "mesecons_torch:mesecon_torch_on", 0, 5, 3, 2, 4, 1);

// TODO: Up/Down orienations
// Note: power state is ignored, since it resets quickly.  See also: wooden button
// N:4, S:5, W:3, E:2, Up:0, Down:1
CONV_DP(77, "minecraft:stone_button", "1,9,0,8,5,13", "mcl_buttons:button_stone_off", 3);
CONV_DP(77, "minecraft:stone_button", "2,10", "mcl_buttons:button_stone_off", 2);
CONV_DP(77, "minecraft:stone_button", "3,11", "mcl_buttons:button_stone_off", 4);
CONV_DP(77, "minecraft:stone_button", "4,12", "mcl_buttons:button_stone_off", 5);

// Round snow height to nearest node
CONV_D(78, "minecraft:snow_layer", "0", "mcl_core:snow");
CONV_D(78, "minecraft:snow_layer", "1", "mcl_core:snow_2");
CONV_D(78, "minecraft:snow_layer", "2", "mcl_core:snow_3");
CONV_D(78, "minecraft:snow_layer", "3", "mcl_core:snow_4");
CONV_D(78, "minecraft:snow_layer", "4", "mcl_core:snow_5");
CONV_D(78, "minecraft:snow_layer", "5", "mcl_core:snow_6");
CONV_D(78, "minecraft:snow_layer", "6", "mcl_core:snow_7");
CONV_D(78, "minecraft:snow_layer", "7", "mcl_core:snow_8");

CONV(79, "minecraft:ice", "mcl_core:ice");
CONV(80, "minecraft:snow", "mcl_core:snowblock");
CONV(81, "minecraft:cactus", "mcl_core:cactus");
CONV(82, "minecraft:clay", "mcl_core:clay");
CONV(83, "minecraft:reeds", "mcl_core:reeds");

CONV(84, "minecraft:jukebox", "mcl_jukebox:jukebox");

CONV(85, "minecraft:fence", "mcl_fences:oak_fence");

// TODO: Check orientation
CONV_DP(86, "minecraft:pumpkin", "0", "mcl_farming:pumpkin_face", 0);
CONV_DP(86, "minecraft:pumpkin", "1", "mcl_farming:pumpkin_face", 1);
CONV_DP(86, "minecraft:pumpkin", "2", "mcl_farming:pumpkin_face", 2);
CONV_DP(86, "minecraft:pumpkin", "3", "mcl_farming:pumpkin_face", 3);
// TODO: More pumpkin states?

CONV(87, "minecraft:netherrack", "mcl_nether:netherrack");
CONV(88, "minecraft:soul_sand", "mcl_nether:soul_sand");
CONV(89, "minecraft:glowstone", "mcl_nether:glowstone");
// TODO: Orientation not stored
CONV(90, "minecraft:portal", "mcl_portals:portal");

// Jack o'lantern
// TODO: Check orientation
// Ignore "no face" flag
CONV_DP(91, "minecraft:lit_pumpkin", "0,4", "mcl_farming:pumpkin_face_light", 0);
CONV_DP(91, "minecraft:lit_pumpkin", "1,5", "mcl_farming:pumpkin_face_light", 1);
CONV_DP(91, "minecraft:lit_pumpkin", "2,6", "mcl_farming:pumpkin_face_light", 2);
CONV_DP(91, "minecraft:lit_pumpkin", "3,7", "mcl_farming:pumpkin_face_light", 3);

CONV_D(92, "minecraft:cake", "0", "mcl_cake:cake");
CONV_D(92, "minecraft:cake", "1", "mcl_cake:cake_6");
CONV_D(92, "minecraft:cake", "2", "mcl_cake:cake_5");
CONV_D(92, "minecraft:cake", "3", "mcl_cake:cake_4");
CONV_D(92, "minecraft:cake", "4", "mcl_cake:cake_3");
CONV_D(92, "minecraft:cake", "5", "mcl_cake:cake_2");
CONV_D(92, "minecraft:cake", "6", "mcl_cake:cake_1");

// TODO: Check orientation
CONV_DP(93, "minecraft:unpowered_repeater", "0", "mesecons_delayer:delayer_off_1", 1);
CONV_DP(93, "minecraft:unpowered_repeater", "1", "mesecons_delayer:delayer_off_1", 2);
CONV_DP(93, "minecraft:unpowered_repeater", "2", "mesecons_delayer:delayer_off_1", 3);
CONV_DP(93, "minecraft:unpowered_repeater", "3", "mesecons_delayer:delayer_off_1", 0);
CONV_DP(93, "minecraft:unpowered_repeater", "4", "mesecons_delayer:delayer_off_2", 1);
CONV_DP(93, "minecraft:unpowered_repeater", "5", "mesecons_delayer:delayer_off_2", 2);
CONV_DP(93, "minecraft:unpowered_repeater", "6", "mesecons_delayer:delayer_off_2", 3);
CONV_DP(93, "minecraft:unpowered_repeater", "7", "mesecons_delayer:delayer_off_2", 0);
CONV_DP(93, "minecraft:unpowered_repeater", "8", "mesecons_delayer:delayer_off_3", 1);
CONV_DP(93, "minecraft:unpowered_repeater", "9", "mesecons_delayer:delayer_off_3", 2);
CONV_DP(93, "minecraft:unpowered_repeater", "10", "mesecons_delayer:delayer_off_3", 3);
CONV_DP(93, "minecraft:unpowered_repeater", "11", "mesecons_delayer:delayer_off_3", 0);
CONV_DP(93, "minecraft:unpowered_repeater", "12", "mesecons_delayer:delayer_off_4", 1);
CONV_DP(93, "minecraft:unpowered_repeater", "13", "mesecons_delayer:delayer_off_4", 2);
CONV_DP(93, "minecraft:unpowered_repeater", "14", "mesecons_delayer:delayer_off_4", 3);
CONV_DP(93, "minecraft:unpowered_repeater", "15", "mesecons_delayer:delayer_off_4", 0);

CONV_DP(94, "minecraft:powered_repeater", "0", "mesecons_delayer:delayer_on_1", 1);
CONV_DP(94, "minecraft:powered_repeater", "1", "mesecons_delayer:delayer_on_1", 2);
CONV_DP(94, "minecraft:powered_repeater", "2", "mesecons_delayer:delayer_on_1", 3);
CONV_DP(94, "minecraft:powered_repeater", "3", "mesecons_delayer:delayer_on_1", 0);
CONV_DP(94, "minecraft:powered_repeater", "4", "mesecons_delayer:delayer_on_2", 1);
CONV_DP(94, "minecraft:powered_repeater", "5", "mesecons_delayer:delayer_on_2", 2);
CONV_DP(94, "minecraft:powered_repeater", "6", "mesecons_delayer:delayer_on_2", 3);
CONV_DP(94, "minecraft:powered_repeater", "7", "mesecons_delayer:delayer_on_2", 0);
CONV_DP(94, "minecraft:powered_repeater", "8", "mesecons_delayer:delayer_on_3", 1);
CONV_DP(94, "minecraft:powered_repeater", "9", "mesecons_delayer:delayer_on_3", 2);
CONV_DP(94, "minecraft:powered_repeater", "10", "mesecons_delayer:delayer_on_3", 3);
CONV_DP(94, "minecraft:powered_repeater", "11", "mesecons_delayer:delayer_on_3", 0);
CONV_DP(94, "minecraft:powered_repeater", "12", "mesecons_delayer:delayer_on_4", 1);
CONV_DP(94, "minecraft:powered_repeater", "13", "mesecons_delayer:delayer_on_4", 2);
CONV_DP(94, "minecraft:powered_repeater", "14", "mesecons_delayer:delayer_on_4", 3);
CONV_DP(94, "minecraft:powered_repeater", "15", "mesecons_delayer:delayer_on_4", 0);

CONV_D(95, "minecraft:stained_glass", "0", "mcl_core:glass_white");
CONV_D(95, "minecraft:stained_glass", "1", "mcl_core:glass_orange");
CONV_D(95, "minecraft:stained_glass", "2", "mcl_core:glass_magenta");
CONV_D(95, "minecraft:stained_glass", "3", "mcl_core:glass_light_blue");
CONV_D(95, "minecraft:stained_glass", "4", "mcl_core:glass_yellow");
CONV_D(95, "minecraft:stained_glass", "5", "mcl_core:glass_lime");
CONV_D(95, "minecraft:stained_glass", "6", "mcl_core:glass_pink");
CONV_D(95, "minecraft:stained_glass", "7", "mcl_core:glass_gray");
CONV_D(95, "minecraft:stained_glass", "8", "mcl_core:glass_silver");
CONV_D(95, "minecraft:stained_glass", "9", "mcl_core:glass_cyan");
CONV_D(95, "minecraft:stained_glass", "10", "mcl_core:glass_purple");
CONV_D(95, "minecraft:stained_glass", "11", "mcl_core:glass_blue");
CONV_D(95, "minecraft:stained_glass", "12", "mcl_core:glass_brown");
CONV_D(95, "minecraft:stained_glass", "13", "mcl_core:glass_green");
CONV_D(95, "minecraft:stained_glass", "14", "mcl_core:glass_red");
CONV_D(95, "minecraft:stained_glass", "15", "mcl_core:glass_black");

CONV_TRAPDOOR(96, "minecraft:trapdoor", "mcl_doors:trapdoor");

// Silverfish blocks
CONV_D(97, "minecraft:monster_egg", "0", "mcl_monster_eggs:monster_egg_stone");
CONV_D(97, "minecraft:monster_egg", "1", "mcl_monster_eggs:monster_egg_cobble");
CONV_D(97, "minecraft:monster_egg", "2", "mcl_monster_eggs:monster_egg_stonebrick");
CONV_D(97, "minecraft:monster_egg", "3", "mcl_monster_eggs:monster_egg_stonebrickmossy");
CONV_D(97, "minecraft:monster_egg", "4", "mcl_monster_eggs:monster_egg_stonebrickcracked");
CONV_D(97, "minecraft:monster_egg", "5", "mcl_monster_eggs:monster_egg_stonebrickcarved");

CONV_D(98, "minecraft:stonebrick", "0", "mcl_core:stonebrick");
CONV_D(98, "minecraft:stonebrick", "1", "mcl_core:stonebrickmossy");
CONV_D(98, "minecraft:stonebrick", "2", "mcl_core:stonebrickcracked");
CONV_D(98, "minecraft:stonebrick", "3", "mcl_core:stonebrickcarved");

// 99: Brown mushroom block (TODO)
// 100: Red mushroom block (TODO)

CONV(101, "minecraft:iron_bars", "mcl_panes:bar");
CONV(102, "minecraft:glass_pane", "mcl_panes:pane_natural"); // TODO: glass not connected to walls

CONV(103, "minecraft:melon_block", "mcl_farming:melon");

// 104: Pumpkin stem (TODO)
// 105: Melon stem (TODO)

// TODO: check orientation
CONV_DP(106, "minecraft:vine", "1", "mcl_core:vine", 5);
CONV_DP(106, "minecraft:vine", "2", "mcl_core:vine", 3);
CONV_DP(106, "minecraft:vine", "5", "mcl_core:vine", 2); // fixme: not "5" or "3"
CONV_DP(106, "minecraft:vine", "4", "mcl_core:vine", 4);
// missing 106:0
// missing 106:10
// missing 106:11
// missing 106:12
// missing 106:13
// missing 106:14
// missing 106:15
// missing 106:3
// missing 106:6
// missing 106:7
// missing 106:8
// missing 106:9


CONV_GATE(107, "minecraft:fence_gate", "mcl_fences:oak_fence_gate");

CONV_STAIR(108, "minecraft:brick_stairs", "mcl_stairs:stair_brick_block");

CONV_STAIR(109, "minecraft:stone_brick_stairs", "mcl_stairs:stair_stonebrick");

CONV(110, "minecraft:mycelium", "mcl_core:mycelium");

CONV(111, "minecraft:waterlily", "mcl_flowers:waterlily");  // TODO: facedir?

CONV(112, "minecraft:nether_brick", "mcl_nether:nether_brick");
CONV(113, "minecraft:nether_brick_fence", "mcl_fences:nether_brick_fence");
CONV_STAIR(114, "minecraft:nether_brick_stairs", "mcl_stairs:stair_nether_brick");

// 115: Nether wart (TODO!!!)

CONV_DP(116, "minecraft:enchanting_table", "2", "mcl_enchanting:table", 0);  // N
CONV_DP(116, "minecraft:enchanting_table", "3", "mcl_enchanting:table", 2);  // S
CONV_DP(116, "minecraft:enchanting_table", "4", "mcl_enchanting:table", 3);  // W
CONV_DP(116, "minecraft:enchanting_table", "5", "mcl_enchanting:table", 1);  // E

// 117: Brewing stand (TODO) --> 117:0 , 117:4

CONV_D(118, "minecraft:cauldron", "0", "mcl_cauldrons:cauldron");
CONV_D(118, "minecraft:cauldron", "1", "mcl_cauldrons:cauldron_1");  // TODO: should be 1/3 full
CONV_D(118, "minecraft:cauldron", "2", "mcl_cauldrons:cauldron_2");  // TODO: should be 2/3 full
CONV_D(118, "minecraft:cauldron", "3", "mcl_cauldrons:cauldron_3");  // TODO: should be full

CONV(119, "minecraft:end_portal", "mcl_portals:portal_end");

// 120: End portal frame

CONV(121, "minecraft:end_stone", "mcl_end:end_stone");
CONV(122, "minecraft:dragon_egg", "mcl_end:dragon_egg");

CONV(123, "minecraft:redstone_lamp", "mcl_redstone_lamp:lamp_off");
CONV(124, "minecraft:lit_redstone_lamp", "mcl_redstone_lamp:lamp_on");

CONV_D(125, "minecraft:double_wooden_slab", "0", "mcl_stairs:slab_oak_double");
CONV_D(125, "minecraft:double_wooden_slab", "1", "mcl_stairs:slab_spruce_double");
CONV_D(125, "minecraft:double_wooden_slab", "2", "mcl_stairs:slab_birch_double");
CONV_D(125, "minecraft:double_wooden_slab", "3", "mcl_stairs:slab_jungle_double");
CONV_D(125, "minecraft:double_wooden_slab", "4", "mcl_stairs:slab_acacia_double");
CONV_D(125, "minecraft:double_wooden_slab", "5", "mcl_stairs:slab_dark_oak_double");

CONV_SLAB(126, "minecraft:wooden_slab", 0, 8, "mcl_stairs:slab_oak");
CONV_SLAB(126, "minecraft:wooden_slab", 1, 9, "mcl_stairs:slab_spruce");
CONV_SLAB(126, "minecraft:wooden_slab", 2, 10, "mcl_stairs:slab_birch");
CONV_SLAB(126, "minecraft:wooden_slab", 3, 11, "mcl_stairs:slab_jungle");
CONV_SLAB(126, "minecraft:wooden_slab", 4, 12, "mcl_stairs:slab_acacia");
CONV_SLAB(126, "minecraft:wooden_slab", 5, 13, "mcl_stairs:slab_dark_oak");

// 127: Cocoa (todo!!!)

CONV_STAIR(128, "minecraft:sandstone_stairs", "mcl_stairs:stair_sandstone");

CONV(129, "minecraft:emerald_ore", "mcl_core:stone_with_emerald");

// 130: Ender chest
CONV_DP(130, "minecraft:ender_chest", "2", "mcl_chests:ender_chest_small", 2);
CONV_DP(130, "minecraft:ender_chest", "3", "mcl_chests:ender_chest_small", 0);
CONV_DP(130, "minecraft:ender_chest", "4", "mcl_chests:ender_chest_small", 1);
CONV_DP(130, "minecraft:ender_chest", "5", "mcl_chests:ender_chest_small", 3);

// 131: Tripwire hook
// 132: Tripwire

CONV(133, "minecraft:emerald_block", "mcl_core:emeraldblock");

CONV_STAIR(134, "minecraft:spruce_stairs", "mcl_stairs:stair_spruce");
CONV_STAIR(135, "minecraft:birch_stairs", "mcl_stairs:stair_birch");
CONV_STAIR(136, "minecraft:jungle_stairs", "mcl_stairs:stair_jungle");

CONV(137, "minecraft:command_block", "mcl_commandblock:commandblock_off"); // TODO (but probably not)
CONV(138, "minecraft:beacon", "mcl_beacons:beacon_beam");

// TODO: walls are not connected
CONV_D(139, "minecraft:cobblestone_wall", "0", "mcl_walls:cobble");
CONV_D(139, "minecraft:cobblestone_wall", "1", "mcl_walls:mossycobble");

// TODO: check
CONV(140, "minecraft:flower_pot", "mcl_flowerpots:flower_pot");

CONV_D(141, "minecraft:carrots", "0", "mcl_farming:carrot_1");
CONV_D(141, "minecraft:carrots", "1", "mcl_farming:carrot_2");
CONV_D(141, "minecraft:carrots", "2", "mcl_farming:carrot_3");
CONV_D(141, "minecraft:carrots", "3", "mcl_farming:carrot_4");
CONV_D(141, "minecraft:carrots", "4", "mcl_farming:carrot_5");
CONV_D(141, "minecraft:carrots", "5", "mcl_farming:carrot_6");
CONV_D(141, "minecraft:carrots", "6", "mcl_farming:carrot_7");
CONV_D(141, "minecraft:carrots", "7", "mcl_farming:carrot");

CONV_D(142, "minecraft:potatoes", "0", "mcl_farming:potato_1");
CONV_D(142, "minecraft:potatoes", "1", "mcl_farming:potato_2");
CONV_D(142, "minecraft:potatoes", "2", "mcl_farming:potato_3");
CONV_D(142, "minecraft:potatoes", "3", "mcl_farming:potato_4");
CONV_D(142, "minecraft:potatoes", "4", "mcl_farming:potato_5");
CONV_D(142, "minecraft:potatoes", "5", "mcl_farming:potato_6");
CONV_D(142, "minecraft:potatoes", "6", "mcl_farming:potato_7");
CONV_D(142, "minecraft:potatoes", "7", "mcl_farming:potato");

// TODO: Up/Down orienations (requires mesecons support)
// Note: power state is ignored, since it resets quickly.  See also: stone button
CONV_DP(143, "minecraft:wooden_button", "1,9,0,8,5,13", "mcl_buttons:button_oak_off", 3);
CONV_DP(143, "minecraft:wooden_button", "2,10", "mcl_buttons:button_oak_off", 2);
CONV_DP(143, "minecraft:wooden_button", "3,11", "mcl_buttons:button_oak_off", 4);
CONV_DP(143, "minecraft:wooden_button", "4,12", "mcl_buttons:button_oak_off", 5);

// 144: Mob head

CONV(145, "minecraft:anvil", "mcl_anvils:anvil");

// Trapped chest
CONV_DP(146, "minecraft:trapped_chest", "2", "mcl_chests:trapped_chest", 2); // N facing
CONV_DP(146, "minecraft:trapped_chest", "3", "mcl_chests:trapped_chest", 0); // S facing
CONV_DP(146, "minecraft:trapped_chest", "4", "mcl_chests:trapped_chest", 1); // W facing
CONV_DP(146, "minecraft:trapped_chest", "5", "mcl_chests:trapped_chest", 3); // E facing

// TODO
CONV(147, "minecraft:light_weighted_pressure_plate", "mcl_pressureplates:pressure_plate_stone_off");
CONV(148, "minecraft:heavy_weighted_pressure_plate", "mcl_pressureplates:pressure_plate_stone_off");

// TODO: 149: Redstone comparator (off) (logic gates)
// 150: Redstone comparator (on)
// Solar panel
CONV(151, "minecraft:daylight_detector", "mcl_daylight_detector:daylight_detector");

CONV(152, "minecraft:redstone_block", "mesecons_torch:redstoneblock");

CONV(153, "minecraft:quartz_ore", "mcl_nether:quartz_ore");

// TODO check orientation / variants
CONV(154, "minecraft:hopper", "mcl_hoppers:hopper");

CONV_D(155, "minecraft:quartz_block", "0", "mcl_nether:quartz_block");
CONV_D(155, "minecraft:quartz_block", "1", "mcl_nether:quartz_chiseled");
CONV_D(155, "minecraft:quartz_block", "2", "mcl_nether:quartz_pillar");
// TODO: "check that"
CONV_DP(155, "minecraft:quartz_block", "3", "mcl_nether:quartz_pillar", 4);  // North/South pilar
CONV_DP(155, "minecraft:quartz_block", "4", "mcl_nether:quartz_pillar", 12);  // East/West pillar

CONV_STAIR(156, "minecraft:quartz_stairs", "mcl_stairs:stair_quartzblock");

CONV(157, "minecraft:activator_rail", "mcl_minecarts:activator_rail");

// 158: Dropper

CONV_D(159, "minecraft:stained_hardened_clay", "0", "mcl_colorblocks:hardened_clay_white");
CONV_D(159, "minecraft:stained_hardened_clay", "1", "mcl_colorblocks:hardened_clay_orange");
CONV_D(159, "minecraft:stained_hardened_clay", "2", "mcl_colorblocks:hardened_clay_magenta");
CONV_D(159, "minecraft:stained_hardened_clay", "3", "mcl_colorblocks:hardened_clay_light_blue");
CONV_D(159, "minecraft:stained_hardened_clay", "4", "mcl_colorblocks:hardened_clay_yellow");
CONV_D(159, "minecraft:stained_hardened_clay", "5", "mcl_colorblocks:hardened_clay_lime");
CONV_D(159, "minecraft:stained_hardened_clay", "6", "mcl_colorblocks:hardened_clay_pink");
CONV_D(159, "minecraft:stained_hardened_clay", "7", "mcl_colorblocks:hardened_clay_grey");
CONV_D(159, "minecraft:stained_hardened_clay", "8", "mcl_colorblocks:hardened_clay_silver");
CONV_D(159, "minecraft:stained_hardened_clay", "9", "mcl_colorblocks:hardened_clay_cyan");
CONV_D(159, "minecraft:stained_hardened_clay", "10", "mcl_colorblocks:hardened_clay_purple");
CONV_D(159, "minecraft:stained_hardened_clay", "11", "mcl_colorblocks:hardened_clay_blue");
CONV_D(159, "minecraft:stained_hardened_clay", "12", "mcl_colorblocks:hardened_clay_brown");
CONV_D(159, "minecraft:stained_hardened_clay", "13", "mcl_colorblocks:hardened_clay_green");
CONV_D(159, "minecraft:stained_hardened_clay", "14", "mcl_colorblocks:hardened_clay_red");
CONV_D(159, "minecraft:stained_hardened_clay", "15", "mcl_colorblocks:hardened_clay_black");

// TODO: orientation needs fixing
CONV_D(160, "minecraft:stained_glass_pane", "0", "mcl_panes:pane_white_flat");
CONV_D(160, "minecraft:stained_glass_pane", "1", "mcl_panes:pane_orange_flat");
CONV_D(160, "minecraft:stained_glass_pane", "2", "mcl_panes:pane_magenta_flat");
CONV_D(160, "minecraft:stained_glass_pane", "3", "mcl_panes:pane_light_blue_flat");
CONV_D(160, "minecraft:stained_glass_pane", "4", "mcl_panes:pane_yellow_flat");
CONV_D(160, "minecraft:stained_glass_pane", "5", "mcl_panes:pane_lime_flat");
CONV_D(160, "minecraft:stained_glass_pane", "6", "mcl_panes:pane_pink_flat");
CONV_D(160, "minecraft:stained_glass_pane", "7", "mcl_panes:pane_grey_flat");
CONV_D(160, "minecraft:stained_glass_pane", "8", "mcl_panes:pane_silver_flat");
CONV_D(160, "minecraft:stained_glass_pane", "9", "mcl_panes:pane_cyan_flat");
CONV_D(160, "minecraft:stained_glass_pane", "10", "mcl_panes:pane_purple_flat");
CONV_D(160, "minecraft:stained_glass_pane", "11", "mcl_panes:pane_blue_flat");
CONV_D(160, "minecraft:stained_glass_pane", "12", "mcl_panes:pane_brown_flat");
CONV_D(160, "minecraft:stained_glass_pane", "13", "mcl_panes:pane_green_flat");
CONV_D(160, "minecraft:stained_glass_pane", "14", "mcl_panes:pane_red_flat");
CONV_D(160, "minecraft:stained_glass_pane", "15", "mcl_panes:pane_black_flat");

CONV_D(161, "minecraft:leaves2", "0,8", "mcl_trees:leaves_acacia");
CONV_D(161, "minecraft:leaves2", "1,9", "mcl_trees:leaves_dark_oak");
CONV_DP(161, "minecraft:leaves2", "4,12", "mcl_trees:leaves_acacia", 1);
CONV_DP(161, "minecraft:leaves2", "5,13", "mcl_trees:leaves_dark_oak", 1);
CONV_LOG(162, "minecraft:log2", 0, "mcl_trees:tree_acacia");
CONV_LOG(162, "minecraft:log2", 1, "mcl_trees:tree_dark_oak");

CONV_STAIR(163, "minecraft:acacia_stairs", "mcl_stairs:stair_acacia");
CONV_STAIR(164, "minecraft:dark_oak_stairs", "mcl_stairs:stair_dark_oak");

CONV(165, "minecraft:slime_block", "mcl_core:slimeblock");

CONV(166, "minecraft:barrier", "mcl_core:barrier");

CONV_TRAPDOOR(167, "minecraft:iron_trapdoor", "mcl_doors:iron_trapdoor");

CONV_D(168, "minecraft:prismarine", "0", "mcl_ocean:prismarine");
CONV_D(168, "minecraft:prismarine", "1", "mcl_ocean:prismarine_brick");
CONV_D(168, "minecraft:prismarine", "2", "mcl_ocean:prismarine_dark");

CONV(169, "minecraft:sea_lantern", "mcl_ocean:sea_lantern");

CONV(170, "minecraft:hay_block", "mcl_farming:hay_block");
CONV_DP(170, "minecraft:hay_block", "0", "mcl_farming:hay_block", 0);
CONV_DP(170, "minecraft:hay_block", "4", "mcl_farming:hay_block", 12);
CONV_DP(170, "minecraft:hay_block", "8", "mcl_farming:hay_block", 6);

CONV_D(171, "minecraft:carpet", "0", "mcl_wool:white_carpet");
CONV_D(171, "minecraft:carpet", "1", "mcl_wool:orange_carpet");
CONV_D(171, "minecraft:carpet", "2", "mcl_wool:magenta_carpet");
CONV_D(171, "minecraft:carpet", "3", "mcl_wool:light_blue_carpet");
CONV_D(171, "minecraft:carpet", "4", "mcl_wool:yellow_carpet");
CONV_D(171, "minecraft:carpet", "5", "mcl_wool:lime_carpet");
CONV_D(171, "minecraft:carpet", "6", "mcl_wool:pink_carpet");
CONV_D(171, "minecraft:carpet", "7", "mcl_wool:grey_carpet");
CONV_D(171, "minecraft:carpet", "8", "mcl_wool:silver_carpet");
CONV_D(171, "minecraft:carpet", "9", "mcl_wool:cyan_carpet");
CONV_D(171, "minecraft:carpet", "10", "mcl_wool:purple_carpet");
CONV_D(171, "minecraft:carpet", "11", "mcl_wool:blue_carpet");
CONV_D(171, "minecraft:carpet", "12", "mcl_wool:brown_carpet");
CONV_D(171, "minecraft:carpet", "13", "mcl_wool:green_carpet");
CONV_D(171, "minecraft:carpet", "14", "mcl_wool:red_carpet");
CONV_D(171, "minecraft:carpet", "15", "mcl_wool:black_carpet");

CONV(172, "minecraft:hardened_clay", "mcl_colorblocks:hardened_clay");
CONV(173, "minecraft:coal_block", "mcl_core:coalblock");
CONV(174, "minecraft:packed_ice", "mcl_core:packed_ice");

CONV_D(175, "minecraft:double_plant", "0", "mcl_flowers:sunflower");
CONV_D(175, "minecraft:double_plant", "1", "mcl_flowers:lilac");
CONV_DP_CB(175, "minecraft:double_plant", "2", "mcl_flowers:double_grass", 0, update_node_light);
CONV_D(175, "minecraft:double_plant", "3", "mcl_flowers:double_fern");
CONV_D(175, "minecraft:double_plant", "4", "mcl_flowers:rose_bush");
CONV_D(175, "minecraft:double_plant", "5", "mcl_flowers:peony");

// "Bug in minecraft maps all top halves [of flowers] to '175 10'"
// TODO: investigate this

// 176: Standing banner
// 177: Wall banner

CONV(178, "minecraft:daylight_detector_inverted", "mesecons_solarpanel:solar_panel_off");

CONV_D(179, "minecraft:red_sandstone", "0", "mcl_core:redsandstone");  // Chisled/smooth
CONV_D(179, "minecraft:red_sandstone", "1", "mcl_core:redsandstonecarved");
CONV_D(179, "minecraft:red_sandstone", "2", "mcl_core:redsandstonesmooth");

CONV_STAIR(180, "minecraft:red_sandstone_stairs", "mcl_stairs:stair_redsandstone");
CONV(181, "minecraft:double_stone_slab2", "mcl_core:redsandstone");
CONV_SLAB(182, "minecraft:stone_slab2", 0, 8, "mcl_stairs:slab_redsandstone");

CONV_GATE(183, "minecraft:spruce_fence_gate", "mcl_fences:spruce_fence_gate");
CONV_GATE(184, "minecraft:birch_fence_gate", "mcl_fences:birch_fence_gate");
CONV_GATE(185, "minecraft:jungle_fence_gate", "mcl_fences:jungle_fence_gate");
CONV_GATE(186, "minecraft:dark_oak_fence_gate", "mcl_fences:dark_oak_fence_gate");
CONV_GATE(187, "minecraft:acacia_fence_gate", "mcl_fences:acacia_fence_gate");

CONV(188, "minecraft:spruce_fence", "mcl_fences:spruce_fence");
CONV(189, "minecraft:birch_fence", "mcl_fences:birch_fence");
CONV(190, "minecraft:jungle_fence", "mcl_fences:jungle_fence");
CONV(191, "minecraft:dark_oak_fence", "mcl_fences:dark_oak_fence");
CONV(192, "minecraft:acacia_fence", "mcl_fences:acacia_fence");

CONV_DOOR(193, "minecraft:spruce_door", "mcl_doors:door_spruce");
CONV_DOOR(194, "minecraft:birch_door", "mcl_doors:door_birch");
CONV_DOOR(195, "minecraft:jungle_door", "mcl_doors:door_jungle");
CONV_DOOR(196, "minecraft:acacia_door", "mcl_doors:door_acacia");
CONV_DOOR(197, "minecraft:dark_oak_door", "mcl_doors:door_dark_oak");

// TODO: rotation
CONV(198, "minecraft:end_rod", "mcl_end:end_rod");
CONV(199, "minecraft:chorus_plant", "mcl_end:chorus_plant");
CONV(200, "minecraft:chorus_flower", "mcl_end:chorus_flower"); // TODO: chorus flower can die?
CONV(201, "minecraft:purpur_block", "mcl_end:purpur_block");

// TODO: rotation
CONV(202, "minecraft:purpur_pillar", "mcl_end:purpur_pillar");

CONV_STAIR(203, "minecraft:purpur_stairs", "mcl_stairs:stair_purpur_block");

CONV(204, "minecraft:purpur_double_slab", "mcl_stairs:slab_purpur_block");
CONV_SLAB(205, "minecraft:purpur_slab", 0, 8, "mcl_stairs:slab_purpur_block");

CONV(206, "minecraft:end_bricks", "mcl_end:end_bricks");

CONV_D(207, "minecraft:beetroots", "0", "mcl_farming:beetroot_0");
CONV_D(207, "minecraft:beetroots", "1", "mcl_farming:beetroot_1");
CONV_D(207, "minecraft:beetroots", "2", "mcl_farming:beetroot_2");
CONV_D(207, "minecraft:beetroots", "3", "mcl_farming:beetroot");

CONV(208, "minecraft:grass_path", "mcl_core:grass_path");

// 209: End gateway
// 210: Repeating command block
// 211: Chain command block

CONV_D(212, "minecraft:frosted_ice", "0", "mcl_core:frosted_ice_0");
CONV_D(212, "minecraft:frosted_ice", "1", "mcl_core:frosted_ice_1");
CONV_D(212, "minecraft:frosted_ice", "2", "mcl_core:frosted_ice_2");
CONV_D(212, "minecraft:frosted_ice", "3", "mcl_core:frosted_ice_3");

CONV(213, "minecraft:magma", "mcl_nether:magma");
CONV(214, "minecraft:nether_wart_block", "mcl_nether:nether_wart_block");
CONV(215, "minecraft:red_nether_brick", "mcl_nether:red_nether_brick");

CONV_DP(216, "minecraft:bone_block", "0", "mcl_core:bone_block", 0);
CONV_DP(216, "minecraft:bone_block", "4", "mcl_core:bone_block", 12);
CONV_DP(216, "minecraft:bone_block", "8", "mcl_core:bone_block", 6);

// 217: Structure Void
// 218: Observer
// 219-234: Shulker Box (coloured variants)
CONV(219, "minecraft:white_shulker_box", "mcl_chests:white_shulker_box_small");
CONV(220, "minecraft:orange_shulker_box", "mcl_chests:orange_shulker_box_small");
CONV(221, "minecraft:magenta_shulker_box", "mcl_chests:magenta_shulker_box_small");
CONV(222, "minecraft:light_blue_shulker_box", "mcl_chests:lightblue_shulker_box_small");
CONV(223, "minecraft:yellow_shulker_box", "mcl_chests:yellow_shulker_box_small");
CONV(224, "minecraft:lime_shulker_box", "mcl_chests:green_shulker_box_small");
CONV(225, "minecraft:pink_shulker_box", "mcl_chests:pink_shulker_box_small");
CONV(226, "minecraft:gray_shulker_box", "mcl_chests:dark_grey_shulker_box_small");
CONV(227, "minecraft:silver_shulker_box", "mcl_chests:grey_shulker_box_small");
CONV(228, "minecraft:cyan_shulker_box", "mcl_chests:cyan_shulker_box_small");
CONV(229, "minecraft:purple_shulker_box", "mcl_chests:violet_shulker_box_small");
CONV(230, "minecraft:blue_shulker_box", "mcl_chests:blue_shulker_box_small");
CONV(231, "minecraft:brown_shulker_box", "mcl_chests:brown_shulker_box_small");
CONV(232, "minecraft:green_shulker_box", "mcl_chests:dark_green_shulker_box_small");
CONV(233, "minecraft:red_shulker_box", "mcl_chests:red_shulker_box_small");
CONV(234, "minecraft:black_shulker_box", "mcl_chests:black_shulker_box_small");

// 235-250: Glazed Terracotta (coloured variants)

CONV_D(251, "minecraft:concrete", "0", "mcl_colorblocks:concrete_white");
CONV_D(251, "minecraft:concrete", "1", "mcl_colorblocks:concrete_orange");
CONV_D(251, "minecraft:concrete", "2", "mcl_colorblocks:concrete_magenta");
CONV_D(251, "minecraft:concrete", "3", "mcl_colorblocks:concrete_light_blue");
CONV_D(251, "minecraft:concrete", "4", "mcl_colorblocks:concrete_yellow");
CONV_D(251, "minecraft:concrete", "5", "mcl_colorblocks:concrete_lime");
CONV_D(251, "minecraft:concrete", "6", "mcl_colorblocks:concrete_pink");
CONV_D(251, "minecraft:concrete", "7", "mcl_colorblocks:concrete_grey");
CONV_D(251, "minecraft:concrete", "8", "mcl_colorblocks:concrete_silver");
CONV_D(251, "minecraft:concrete", "9", "mcl_colorblocks:concrete_cyan");
CONV_D(251, "minecraft:concrete", "10", "mcl_colorblocks:concrete_purple");
CONV_D(251, "minecraft:concrete", "11", "mcl_colorblocks:concrete_blue");
CONV_D(251, "minecraft:concrete", "12", "mcl_colorblocks:concrete_brown");
CONV_D(251, "minecraft:concrete", "13", "mcl_colorblocks:concrete_green");
CONV_D(251, "minecraft:concrete", "14", "mcl_colorblocks:concrete_red");
CONV_D(251, "minecraft:concrete", "15", "mcl_colorblocks:concrete_black");

CONV_D(252, "minecraft:concrete_powder", "0", "mcl_colorblocks:concrete_powder_white");
CONV_D(252, "minecraft:concrete_powder", "1", "mcl_colorblocks:concrete_powder_orange");
CONV_D(252, "minecraft:concrete_powder", "2", "mcl_colorblocks:concrete_powder_magenta");
CONV_D(252, "minecraft:concrete_powder", "3", "mcl_colorblocks:concrete_powder_light_blue");
CONV_D(252, "minecraft:concrete_powder", "4", "mcl_colorblocks:concrete_powder_yellow");
CONV_D(252, "minecraft:concrete_powder", "5", "mcl_colorblocks:concrete_powder_lime");
CONV_D(252, "minecraft:concrete_powder", "6", "mcl_colorblocks:concrete_powder_pink");
CONV_D(252, "minecraft:concrete_powder", "7", "mcl_colorblocks:concrete_powder_grey");
CONV_D(252, "minecraft:concrete_powder", "8", "mcl_colorblocks:concrete_powder_silver");
CONV_D(252, "minecraft:concrete_powder", "9", "mcl_colorblocks:concrete_powder_cyan");
CONV_D(252, "minecraft:concrete_powder", "10", "mcl_colorblocks:concrete_powder_purple");
CONV_D(252, "minecraft:concrete_powder", "11", "mcl_colorblocks:concrete_powder_blue");
CONV_D(252, "minecraft:concrete_powder", "12", "mcl_colorblocks:concrete_powder_brown");
CONV_D(252, "minecraft:concrete_powder", "13", "mcl_colorblocks:concrete_powder_green");
CONV_D(252, "minecraft:concrete_powder", "14", "mcl_colorblocks:concrete_powder_red");
CONV_D(252, "minecraft:concrete_powder", "15", "mcl_colorblocks:concrete_powder_black");

// 253 and 254 are not used by 1.12 (and 1.13 gets rid of numeric block IDs altogether)

// 255: Structure block
