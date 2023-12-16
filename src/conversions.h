
#define CONV_ALL(id, mcn, datas, mtn, p2, is_tool, cb) \
	static_assert(id <= MC_ID_MAX, "MC ID is out of range! (update MC_ID_MAX if adding new items)"); \
	add_conversion(id, mcn, datas, mtn, p2, is_tool, cb)
#define CONV(id, mcn, mtn) CONV_D(id, mcn, nullptr, mtn)
#define CONV_D(id, mcn, datas, mtn) CONV_DP(id, mcn, datas, mtn, 0)
#define CONV_P(id, mcn, mtn, p2); CONV_DP(id, mcn, nullptr, mtn, p2)
#define CONV_DP(id, mcn, datas, mtn, p2) CONV_DP_CB(id, mcn, datas, mtn, p2, nullptr)
#define CONV_DP_CB(id, mcn, datas, mtn, p2, cb) CONV_ALL(id, mcn, datas, mtn, p2, false, cb)
#define CONV_TOOL(id, mcn, mtn) CONV_ALL(id, mcn, nullptr, mtn, 0, true, nullptr)

#define CONV_STAIR(id, mcn, mtn) \
	CONV_DP_CB(id, mcn, "0", mtn, 3, update_node_light); \
	CONV_DP_CB(id, mcn, "1", mtn, 1, update_node_light); \
	CONV_DP_CB(id, mcn, "2", mtn, 0, update_node_light); \
	CONV_DP_CB(id, mcn, "3", mtn, 2, update_node_light); \
	CONV_DP_CB(id, mcn, "4", mtn, 21, update_node_light); \
	CONV_DP_CB(id, mcn, "5", mtn, 23, update_node_light); \
	CONV_DP_CB(id, mcn, "6", mtn, 20, update_node_light); \
	CONV_DP_CB(id, mcn, "7", mtn, 22, update_node_light)

#define CONV_SLAB(id, mcn, dbottom, dtop, mtn) \
	CONV_DP_CB(id, mcn, #dbottom, mtn, 0, update_node_light); \
	CONV_DP_CB(id, mcn, #dtop, mtn, 22, update_node_light)

#define CONV_TRAPDOOR(id, mcn, mtn) \
	CONV_DP(id, mcn, "0", mtn, 0); \
	CONV_DP(id, mcn, "1", mtn, 2); \
	CONV_DP(id, mcn, "2", mtn, 3); \
	CONV_DP(id, mcn, "3", mtn, 1); \
	CONV_DP(id, mcn, "4", mtn "_open", 0); \
	CONV_DP(id, mcn, "5", mtn "_open", 2); \
	CONV_DP(id, mcn, "6", mtn "_open", 3); \
	CONV_DP(id, mcn, "7", mtn "_open", 1); \
	CONV_DP(id, mcn, "8", mtn, 20); \
	CONV_DP(id, mcn, "9", mtn, 22); \
	CONV_DP(id, mcn, "10", mtn, 23); \
	CONV_DP(id, mcn, "11", mtn, 21); \
	CONV_DP(id, mcn, "12", mtn "_open", 20); \
	CONV_DP(id, mcn, "13", mtn "_open", 22); \
	CONV_DP(id, mcn, "14", mtn "_open", 23); \
	CONV_DP(id, mcn, "15", mtn "_open", 21)

#define CONV_LOG(id, mcn, d, mtn) \
	CONV_DP(id, mcn, #d,                                   mtn, 0); \
	CONV_DP(id, mcn, std::to_string(d + (1 << 2)).c_str(), mtn, 12); \
	CONV_DP(id, mcn, std::to_string(d + (2 << 2)).c_str(), mtn, 4); \
	CONV_DP(id, mcn, std::to_string(d + (3 << 2)).c_str(), mtn, 0)

#define CONV_WALLMOUNTED(id, mcn, mtn, dt, db, dn, de, ds, dw) \
	CONV_DP(id, mcn, #dt, mtn, 0); \
	CONV_DP(id, mcn, #db, mtn, 1); \
	CONV_DP(id, mcn, #dn, mtn, 5); \
	CONV_DP(id, mcn, #de, mtn, 3); \
	CONV_DP(id, mcn, #ds, mtn, 4); \
	CONV_DP(id, mcn, #dw, mtn, 2)

// TODO: Check orientation
#define CONV_GATE(id, mcn, mtn) \
	CONV_DP(id, mcn, "0", mtn "_closed", 0); \
	CONV_DP(id, mcn, "1", mtn "_closed", 2); \
	CONV_DP(id, mcn, "2", mtn "_closed", 3); \
	CONV_DP(id, mcn, "3", mtn "_closed", 1); \
	CONV_DP(id, mcn, "4", mtn "_open", 0); \
	CONV_DP(id, mcn, "5", mtn "_open", 2); \
	CONV_DP(id, mcn, "6", mtn "_open", 3); \
	CONV_DP(id, mcn, "7", mtn "_open", 1)

#if MC_VERSION < 102
// TODO: Check orientation
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

// TODO: 23: Dispenser

CONV_D(24, "minecraft:sandstone", "0", "mcl_core:sandstone");
CONV_D(24, "minecraft:sandstone", "1", "mcl_core:sandstonecarved");
CONV_D(24, "minecraft:sandstone", "2", "mcl_core:sandstonesmooth");

CONV(25, "minecraft:noteblock", "mesecons_noteblock:noteblock"); // TODO

CONV_DP(26, "minecraft:bed", "0,4", "mcl_beds:bed_red_bottom", 0); // S
CONV_DP(26, "minecraft:bed", "1,5", "mcl_beds:bed_red_bottom", 1); // W
CONV_DP(26, "minecraft:bed", "2,6", "mcl_beds:bed_red_bottom", 2); // N
CONV_DP(26, "minecraft:bed", "3,7", "mcl_beds:bed_red_bottom", 3); // E
CONV_DP(26, "minecraft:bed", "8,12", "mcl_beds:bed_red_top", 0);  // S
CONV_DP(26, "minecraft:bed", "9,13", "mcl_beds:bed_red_top", 1);  // W
CONV_DP(26, "minecraft:bed", "10,14", "mcl_beds:bed_red_top", 2); // N
CONV_DP(26, "minecraft:bed", "11,15", "mcl_beds:bed_red_top", 3); // E

// TODO?
CONV(27, "minecraft:golden_rail", "mcl_minecarts:golden_rail");  // Powered rail
CONV(28, "minecraft:detector_rail", "mcl_minecarts:detector_rail");

// TODO: Check orientation
CONV_D(29, "minecraft:sticky_piston", "0", "mesecons_pistons:piston_down_sticky_off");
CONV_D(29, "minecraft:sticky_piston", "1", "mesecons_pistons:piston_up_sticky_off");
CONV_DP(29, "minecraft:sticky_piston", "2", "mesecons_pistons:piston_sticky_off", 0);  // N
CONV_DP(29, "minecraft:sticky_piston", "3", "mesecons_pistons:piston_sticky_off", 2);  // S
CONV_DP(29, "minecraft:sticky_piston", "4", "mesecons_pistons:piston_sticky_off", 3);  // W
CONV_DP(29, "minecraft:sticky_piston", "5", "mesecons_pistons:piston_sticky_off", 1);  // E
CONV_D(29, "minecraft:sticky_piston", "8", "mesecons_pistons:piston_down_sticky_on");
CONV_D(29, "minecraft:sticky_piston", "9", "mesecons_pistons:piston_up_sticky_on");
CONV_DP(29, "minecraft:sticky_piston", "10", "mesecons_pistons:piston_sticky_on", 0);  // N
CONV_DP(29, "minecraft:sticky_piston", "11", "mesecons_pistons:piston_sticky_on", 2);  // S
CONV_DP(29, "minecraft:sticky_piston", "12", "mesecons_pistons:piston_sticky_on", 3);  // W
CONV_DP(29, "minecraft:sticky_piston", "13", "mesecons_pistons:piston_sticky_on", 1);  // E

CONV(30, "minecraft:web", "mcl_core:cobweb");

CONV_D(31, "minecraft:tallgrass", "0", "mcl_core:deadbush");
CONV_D(31, "minecraft:tallgrass", "1", "mcl_flowers:tallgrass");
CONV_D(31, "minecraft:tallgrass", "2", "mcl_flowers:fern");

CONV(32, "minecraft:deadbush", "mcl_core:deadbush");

// TODO: Check orientation
CONV_D(33, "minecraft:piston", "0", "mesecons_piston:piston_down_normal_off");
CONV_D(33, "minecraft:piston", "1", "mesecons_piston:piston_up_normal_off");
CONV_DP(33, "minecraft:piston", "2", "mesecons_piston:piston_normal_off", 0);  // N
CONV_DP(33, "minecraft:piston", "3", "mesecons_piston:piston_normal_off", 2);  // S
CONV_DP(33, "minecraft:piston", "4", "mesecons_piston:piston_normal_off", 3);  // W
CONV_DP(33, "minecraft:piston", "5", "mesecons_piston:piston_normal_off", 1);  // E
CONV_D(33, "minecraft:piston", "8", "mesecons_piston:piston_down_normal_on");
CONV_D(33, "minecraft:piston", "9", "mesecons_piston:piston_up_normal_on");
CONV_DP(33, "minecraft:piston", "10", "mesecons_piston:piston_normal_on", 0);  // N
CONV_DP(33, "minecraft:piston", "11", "mesecons_piston:piston_normal_on", 2);  // S
CONV_DP(33, "minecraft:piston", "12", "mesecons_piston:piston_normal_on", 3);  // W
CONV_DP(33, "minecraft:piston", "13", "mesecons_piston:piston_normal_on", 1);  // E

CONV_D(34, "minecraft:piston_head", "0", "mesecons_piston:piston_down_pusher_normal");
CONV_D(34, "minecraft:piston_head", "1", "mesecons_piston:piston_up_pusher_normal");
CONV_DP(34, "minecraft:piston_head", "2", "mesecons_piston:piston_pusher_normal", 0);  // N
CONV_DP(34, "minecraft:piston_head", "3", "mesecons_piston:piston_pusher_normal", 2);  // S
CONV_DP(34, "minecraft:piston_head", "4", "mesecons_piston:piston_pusher_normal", 3);  // W
CONV_DP(34, "minecraft:piston_head", "5", "mesecons_piston:piston_pusher_normal", 1);  // E
CONV_D(34, "minecraft:piston_head", "8", "mesecons_piston:piston_down_pusher_sticky");
CONV_D(34, "minecraft:piston_head", "9", "mesecons_piston:piston_up_pusher_sticky");
CONV_DP(34, "minecraft:piston_head", "10", "mesecons_piston:piston_pusher_sticky", 0);  // N
CONV_DP(34, "minecraft:piston_head", "11", "mesecons_piston:piston_pusher_sticky", 2);  // S
CONV_DP(34, "minecraft:piston_head", "12", "mesecons_piston:piston_pusher_sticky", 3);  // W
CONV_DP(34, "minecraft:piston_head", "13", "mesecons_piston:piston_pusher_sticky", 1);  // E

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
CONV_D(43, "minecraft:double_stone_slab", "1", "mcl_core:sandstone");
CONV_D(43, "minecraft:double_stone_slab", "2", "mcl_trees:wood_oak");
CONV_D(43, "minecraft:double_stone_slab", "3", "mcl_core:cobble");
CONV_D(43, "minecraft:double_stone_slab", "4", "mcl_core:brick_block");
CONV_D(43, "minecraft:double_stone_slab", "5", "mcl_core:stonebrick");
CONV_D(43, "minecraft:double_stone_slab", "6", "mcl_nether:nether_brick");
CONV_D(43, "minecraft:double_stone_slab", "7", "mcl_nether:quartz_block");
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

CONV_WALLMOUNTED(50, "minecraft:torch", "mcl_torches:torch", 0, 5, 3, 2, 4, 1);

CONV(51, "minecraft:fire", "mcl_fire:fire");

CONV(52, "minecraft:mob_spawner", "mcl_mobspawners:spawner"); // TODO

CONV_STAIR(53, "minecraft:oak_stairs", "mcl_stairs:stair_oak");

// TODO
CONV_DP(54, "minecraft:chest", "2", "mcl_chests:chest", 0);  // N
CONV_DP(54, "minecraft:chest", "3", "mcl_chests:chest", 2);  // S
CONV_DP(54, "minecraft:chest", "4", "mcl_chests:chest", 3);  // W
CONV_DP(54, "minecraft:chest", "5", "mcl_chests:chest", 1);  // E

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

CONV_DP(61, "minecraft:furnace", "2", "mcl_furnaces:furnace", 0);  // N
CONV_DP(61, "minecraft:furnace", "3", "mcl_furnaces:furnace", 2);  // S
CONV_DP(61, "minecraft:furnace", "4", "mcl_furnaces:furnace", 3);  // W
CONV_DP(61, "minecraft:furnace", "5", "mcl_furnaces:furnace", 1);  // E
CONV_DP(62, "minecraft:lit_furnace", "2", "mcl_furnaces:furnace_active", 0);
CONV_DP(62, "minecraft:lit_furnace", "3", "mcl_furnaces:furnace_active", 2);
CONV_DP(62, "minecraft:lit_furnace", "4", "mcl_furnaces:furnace_active", 3);
CONV_DP(62, "minecraft:lit_furnace", "5", "mcl_furnaces:furnace_active", 1);

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
CONV_DP(69, "minecraft:lever", "1", "mesecons_walllever:wall_lever_off", 1);  // E
CONV_DP(69, "minecraft:lever", "2", "mesecons_walllever:wall_lever_off", 3);  // W
CONV_DP(69, "minecraft:lever", "3", "mesecons_walllever:wall_lever_off", 2);  // S
CONV_DP(69, "minecraft:lever", "4,0,5,6,7", "mesecons_walllever:wall_lever_off", 0);  // N, up, down
CONV_DP(69, "minecraft:lever", "9", "mesecons_walllever:wall_lever_on", 1);  // E
CONV_DP(69, "minecraft:lever", "10", "mesecons_walllever:wall_lever_on", 3);  // W
CONV_DP(69, "minecraft:lever", "11", "mesecons_walllever:wall_lever_on", 2);  // S
CONV_DP(69, "minecraft:lever", "12,8,13,14,15", "mesecons_walllever:wall_lever_on", 0);  // N, up, down

CONV(70, "minecraft:stone_pressure_plate", "mesecons_pressureplates:pressure_plate_stone_off");

CONV_DOOR(71, "minecraft:iron_door", "mcl_doors:iron_door");

CONV(72, "minecraft:wooden_pressure_plate", "mesecons_pressureplates:pressure_plate_oak_off");

CONV(73, "minecraft:redstone_ore", "mcl_core:stone_with_redstone");
CONV(74, "minecraft:lit_redstone_ore", "mcl_core:stone_with_redstone_lit");

CONV_WALLMOUNTED(75, "minecraft:unlit_redstone_torch", "mesecons_torch:mesecon_torch_off", 0, 5, 3, 2, 4, 1);
CONV_WALLMOUNTED(76, "minecraft:lit_redstone_torch", "mesecons_torch:mesecon_torch_on", 0, 5, 3, 2, 4, 1);

// TODO: Up/Down orienations (requires mesecons support)
// Note: power state is ignored, since it resets quickly.  See also: wooden button
CONV_DP(77, "minecraft:stone_button", "1,9,0,8,5,13", "mesecons_button:button_off", 1);
CONV_DP(77, "minecraft:stone_button", "2,10", "mesecons_button:button_off", 3);
CONV_DP(77, "minecraft:stone_button", "3,11", "mesecons_button:button_off", 2);
CONV_DP(77, "minecraft:stone_button", "4,12", "mesecons_button:button_off", 0);

// Round snow height to nearest node
CONV_D(78, "minecraft:snow_layer", "0", "default:snow");
CONV_D(78, "minecraft:snow_layer", "1", "default:snow_2");
CONV_D(78, "minecraft:snow_layer", "2", "default:snow_3");
CONV_D(78, "minecraft:snow_layer", "3", "default:snow_4");
CONV_D(78, "minecraft:snow_layer", "4", "default:snow_5");
CONV_D(78, "minecraft:snow_layer", "5", "default:snow_6");
CONV_D(78, "minecraft:snow_layer", "6", "default:snow_7");
CONV_D(78, "minecraft:snow_layer", "7", "default:snow_8");

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
// XXX: Orientation not stored
CONV(90, "minecraft:portal", "mcl_portals:portal");

// Jack o'lantern
// TODO: Check orientation
// Ignore "no face" flag
CONV_DP(91, "minecraft:lit_pumpkin", "0,4", "mcl_farming:pumpkin_face_light", 0);
CONV_DP(91, "minecraft:lit_pumpkin", "1,5", "mcl_farming:pumpkin_face_light", 1);
CONV_DP(91, "minecraft:lit_pumpkin", "2,6", "mcl_farming:pumpkin_face_light", 2);
CONV_DP(91, "minecraft:lit_pumpkin", "3,7", "mcl_farming:pumpkin_face_light", 3);

CONV(92, "minecraft:cake", "food:cake"); // TODO

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
CONV(102, "minecraft:glass_pane", "mcl_panes:pane_natural");

CONV(103, "minecraft:melon_block", "mcl_farming:melon");

// 104: Pumpkin stem (TODO)
// 105: Melon stem (TODO)

CONV(106, "minecraft:vine", "mcl_core:vine");  // TODO: Use real vines

CONV_GATE(107, "minecraft:fence_gate", "mcl_fences:oak_fence_gate");

CONV_STAIR(108, "minecraft:brick_stairs", "mcl_stairs:stair_brick_block");

CONV_STAIR(109, "minecraft:stone_brick_stairs", "mcl_stairs:stair_stonebrick");

CONV(110, "minecraft:mycelium", "mcl_core:mycelium");

CONV(111, "minecraft:waterlily", "mcl_flowers:waterlily");  // TODO: facedir?

CONV(112, "minecraft:nether_brick", "mcl_nether:nether_brick");
CONV(113, "minecraft:nether_brick_fence", "mcl_fences:nether_brick_fence");
CONV_STAIR(114, "minecraft:nether_brick_stairs", "mcl_stairs:stair_nether_brick");

// 115: Nether wart (TODO!!!)
// 116: Enchantment table
// 117: Brewing stand

CONV_D(118, "minecraft:cauldron", "0", "mcl_cauldrons:cauldron");
CONV_D(118, "minecraft:cauldron", "1", "mcl_cauldrons:cauldron_1");  // TODO: should be 1/3 full
CONV_D(118, "minecraft:cauldron", "2", "mcl_cauldrons:cauldron_2");  // TODO: should be 2/3 full
CONV_D(118, "minecraft:cauldron", "3", "mcl_cauldrons:cauldron_3");  // TODO: should be full

// 119: End portal
// 120: End portal frame
// 121: End stone
// 122: Dragon egg

CONV(123, "mesecons:redstone_lamp", "mesecons_lightstone:lightstone_off");
CONV(124, "mesecons:lit_redstone_lamp", "mesecons_lightstone:lightstone_on");

CONV_D(125, "minecraft:double_wooden_slab", "0", "mcl_trees:wood_oak");
CONV_D(125, "minecraft:double_wooden_slab", "1", "mcl_trees:wood_spruce");
CONV_D(125, "minecraft:double_wooden_slab", "2", "mcl_trees:wood_birch");
CONV_D(125, "minecraft:double_wooden_slab", "3", "mcl_trees:wood_jungle");
CONV_D(125, "minecraft:double_wooden_slab", "4", "mcl_trees:wood_acacia");
CONV_D(125, "minecraft:double_wooden_slab", "5", "mcl_trees:wood_dark_oak");

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
// 131: Tripwire hook
// 132: Tripwire
// TODO 133: Emerald block

CONV_STAIR(134, "minecraft:spruce_stairs", "mcl_stairs:stair_spruce");
CONV_STAIR(135, "minecraft:birch_stairs", "mcl_stairs:stair_birch");
CONV_STAIR(136, "minecraft:jungle_stairs", "mcl_stairs:stair_jungle");

CONV(137, "minecraft:command_block", "mesecons_commandblock:commandblock_off"); // TODO (but probably not)

// 138: Beacon

CONV_D(139, "minecraft:cobblestone_wall", "0", "mcl_walls:cobble");
CONV_D(139, "minecraft:cobblestone_wall", "1", "mcl_walls:mossycobble");

// 140: Flower pot

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
CONV_DP(143, "minecraft:wooden_button", "1,9,0,8,5,13", "mesecons_button:button_oak_off", 1);
CONV_DP(143, "minecraft:wooden_button", "2,10", "mesecons_button:button_oak_off", 3);
CONV_DP(143, "minecraft:wooden_button", "3,11", "mesecons_button:button_oak_off", 2);
CONV_DP(143, "minecraft:wooden_button", "4,12", "mesecons_button:button_oak_off", 0);

// 144: Mob head

CONV(145, "minecraft:anvil", "mcl_anvils:anvil");

// Trapped chest
CONV_DP(146, "minecraft:trapped_chest", "2", "mcl_chests:trapped_chest", 2);
CONV_DP(146, "minecraft:trapped_chest", "3", "mcl_chests:trapped_chest", 0);
CONV_DP(146, "minecraft:trapped_chest", "4", "mcl_chests:trapped_chest", 1);
CONV_DP(146, "minecraft:trapped_chest", "5", "mcl_chests:trapped_chest", 3);

// TODO
CONV(147, "minecraft:light_weighted_pressure_plate", "mesecons_pressureplates:pressure_plate_stone");
CONV(148, "minecraft:heavy_weighted_pressure_plate", "mesecons_pressureplates:pressure_plate_stone");

// TODO: 149: Redstone comparator (off) (logic gates)
// 150: Redstone comparator (on)
// 151: Solar panel
// 152: Redstone block
// 153: Nether quartz ore
// 154: Hopper

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

// TODO: tinted panes
CONV(160, "minecraft:stained_glass_pane", "mcl_panes:pane_white_flat");

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

// 168: Prismarine
// 169: Sea lantern

CONV(170, "minecraft:hay_block", "mcl_farming:hay_block");

// 171: Carpets

CONV(172, "minecraft:hardened_clay", "mcl_colorblocks:hardened_clay");
CONV(173, "minecraft:coal_block", "mcl_core:coalblock");
CONV(174, "minecraft:packed_ice", "mcl_core:packed_ice");

// TODO
//CONV_D(175, "minecraft:double_plant", "0", "flowers:dandelion_yellow");  // Sunflower
//CONV_D(175, "minecraft:double_plant", "1", "flowers:viola");  // Lilac
//CONV_DP_CB(175, "minecraft:double_plant", "2", "default:junglegrass", 0, update_node_light);  // Double-tall grass
//CONV_D(175, "minecraft:double_plant", "3", "default:junglegrass");  // Large fern
//CONV_D(175, "minecraft:double_plant", "4", "flowers:rose");  // Rose bush
//CONV_D(175, "minecraft:double_plant", "5", "flowers:dandelion_white");  // Peony

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
CONV_GATE(184, "minecraft:birch_fence_gate", "mcl_fences:birch_fence_gat");
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

// 198: End rod
// 199: Chorus plant
// 200: Chorus flower
// 201: Purpur block
// 202: Purpur pillar
// 203: Purpur stairs
// 204: Purpur double slab
// 205: Purpur slab

CONV(206, "minecraft:end_bricks", "mcl_end:end_bricks");

// 207: Beetroot seeds

CONV(208, "minecraft:grass_path", "mcl_core:grass_path");

// 209: End gateway
// 210: Repeating command block
// 211: Chain command block

CONV(212, "minecraft:frosted_ice", "mcl_core:frosted_ice");

// 213-254: Unused
// 255: Structure block
