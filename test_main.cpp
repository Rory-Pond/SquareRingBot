// #include <gtest/gtest.h>

// #include "main.h"

// struct RotationParams {
//     int direction;
//     vec2 expected_position;
// };

// class CardTest : public ::testing::TestWithParam<RotationParams> {};
// class AttackTest;

// TEST(CardTest, MoveTest1) 
// {
// 	Player alice(15, {2,2}, 0, {});
// 	Player bob(15, {0,0}, 0, {});

// 	Board game1(alice,bob);

// 	game1.play_action({Action("AA", "Move_Forwards", 0, 1, {})}, {});
	
// 	EXPECT_EQ(game1.player_red.position, vec2(2,3));
// }

// TEST_P(CardTest, Rotate) {
//     RotationParams params = GetParam();
    
//     Player alice(15, {2,2}, params.direction, {});
//     Player bob(15, {0,0}, 0, {});

//     Board game1(alice, bob);
//     game1.play_action({Action("AA", "Move_Forwards", 0, 1, {})}, {});

//     EXPECT_EQ(game1.player_red.position, params.expected_position);
// }

// TEST(AttackTest, Attack) {
    
//     Player alice(15, {2,2}, 2, {});
//     Player bob(15, {4,2}, 0, {});

//     Board game1(alice, bob);
//     game1.play_action({Action("AA", "Attack_Forwards", 0, 0, {10}, 1)}, {});

//     EXPECT_EQ(game1.player_blue.health, 14);
// }

// // Instantiate the test suite with different test parameters
// INSTANTIATE_TEST_SUITE_P(
//     CardTests,  // Test suite name
//     CardTest,
//     ::testing::Values(
//         RotationParams{0, {2, 3}},
//         RotationParams{1, {3, 3}},
//         RotationParams{2, {3, 2}},
//         RotationParams{3, {3, 1}},
//         RotationParams{4, {2, 1}},
//         RotationParams{5, {1, 1}},
//         RotationParams{6, {1, 2}},
//         RotationParams{7, {1, 3}}
//     )
// );



// int main(int argc, char **argv) {
// 	::testing::InitGoogleTest(&argc, argv);
// 	return RUN_ALL_TESTS();
// }
