#pragma once

#include "Globals.h"
#include "BlockState.h"
#include "Defines.h"

namespace Block
{
	namespace AcaciaButton
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState AcaciaButton(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6442;
					else return 6443;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6444;
					else return 6445;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6446;
					else return 6447;
				else
					if (Powered) return 6448;
					else return 6449;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6450;
					else return 6451;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6452;
					else return 6453;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6454;
					else return 6455;
				else
					if (Powered) return 6456;
					else return 6457;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6458;
					else return 6459;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6460;
					else return 6461;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6462;
					else return 6463;
				else
					if (Powered) return 6464;
					else return 6465;
		}
		BlockState AcaciaButton();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace AcaciaDoor
	{
		enum class Half
		{
			Upper,
			Lower
		};
		enum class Hinge
		{
			Left,
			Right
		};
		constexpr BlockState AcaciaDoor(const eBlockFace Facing, const enum Half Half, const enum Hinge Hinge, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8930;
							else return 8931;
						else
							if (Powered) return 8932;
							else return 8933;
					else
						if (Open)
							if (Powered) return 8934;
							else return 8935;
						else
							if (Powered) return 8936;
							else return 8937;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8938;
							else return 8939;
						else
							if (Powered) return 8940;
							else return 8941;
					else
						if (Open)
							if (Powered) return 8942;
							else return 8943;
						else
							if (Powered) return 8944;
							else return 8945;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8946;
							else return 8947;
						else
							if (Powered) return 8948;
							else return 8949;
					else
						if (Open)
							if (Powered) return 8950;
							else return 8951;
						else
							if (Powered) return 8952;
							else return 8953;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8954;
							else return 8955;
						else
							if (Powered) return 8956;
							else return 8957;
					else
						if (Open)
							if (Powered) return 8958;
							else return 8959;
						else
							if (Powered) return 8960;
							else return 8961;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8962;
							else return 8963;
						else
							if (Powered) return 8964;
							else return 8965;
					else
						if (Open)
							if (Powered) return 8966;
							else return 8967;
						else
							if (Powered) return 8968;
							else return 8969;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8970;
							else return 8971;
						else
							if (Powered) return 8972;
							else return 8973;
					else
						if (Open)
							if (Powered) return 8974;
							else return 8975;
						else
							if (Powered) return 8976;
							else return 8977;
			else
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8978;
							else return 8979;
						else
							if (Powered) return 8980;
							else return 8981;
					else
						if (Open)
							if (Powered) return 8982;
							else return 8983;
						else
							if (Powered) return 8984;
							else return 8985;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8986;
							else return 8987;
						else
							if (Powered) return 8988;
							else return 8989;
					else
						if (Open)
							if (Powered) return 8990;
							else return 8991;
						else
							if (Powered) return 8992;
							else return 8993;
		}
		BlockState AcaciaDoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Hinge Hinge(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace AcaciaFence
	{
		constexpr BlockState AcaciaFence(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 8676;
						else return 8677;
					else
						if (West) return 8680;
						else return 8681;
				else
					if (South)
						if (West) return 8684;
						else return 8685;
					else
						if (West) return 8688;
						else return 8689;
			else
				if (North)
					if (South)
						if (West) return 8692;
						else return 8693;
					else
						if (West) return 8696;
						else return 8697;
				else
					if (South)
						if (West) return 8700;
						else return 8701;
					else
						if (West) return 8704;
						else return 8705;
		}
		BlockState AcaciaFence();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace AcaciaFenceGate
	{
		constexpr BlockState AcaciaFenceGate(const eBlockFace Facing, const bool InWall, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (InWall)
					if (Open)
						if (Powered) return 8514;
						else return 8515;
					else
						if (Powered) return 8516;
						else return 8517;
				else
					if (Open)
						if (Powered) return 8518;
						else return 8519;
					else
						if (Powered) return 8520;
						else return 8521;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (InWall)
					if (Open)
						if (Powered) return 8522;
						else return 8523;
					else
						if (Powered) return 8524;
						else return 8525;
				else
					if (Open)
						if (Powered) return 8526;
						else return 8527;
					else
						if (Powered) return 8528;
						else return 8529;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (InWall)
					if (Open)
						if (Powered) return 8530;
						else return 8531;
					else
						if (Powered) return 8532;
						else return 8533;
				else
					if (Open)
						if (Powered) return 8534;
						else return 8535;
					else
						if (Powered) return 8536;
						else return 8537;
			else
				if (InWall)
					if (Open)
						if (Powered) return 8538;
						else return 8539;
					else
						if (Powered) return 8540;
						else return 8541;
				else
					if (Open)
						if (Powered) return 8542;
						else return 8543;
					else
						if (Powered) return 8544;
						else return 8545;
		}
		BlockState AcaciaFenceGate();
		eBlockFace Facing(BlockState Block);
		bool InWall(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace AcaciaLeaves
	{
		constexpr BlockState AcaciaLeaves(const unsigned char Distance, const bool Persistent)
		{
			if (Distance == 1)
				if (Persistent) return 201;
				else return 202;
			else if (Distance == 2)
				if (Persistent) return 203;
				else return 204;
			else if (Distance == 3)
				if (Persistent) return 205;
				else return 206;
			else if (Distance == 4)
				if (Persistent) return 207;
				else return 208;
			else if (Distance == 5)
				if (Persistent) return 209;
				else return 210;
			else if (Distance == 6)
				if (Persistent) return 211;
				else return 212;
			else
				if (Persistent) return 213;
				else return 214;
		}
		BlockState AcaciaLeaves();
		unsigned char Distance(BlockState Block);
		bool Persistent(BlockState Block);
	}
	namespace AcaciaLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState AcaciaLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 85;
			else if (Axis == Axis::Y) return 86;
			else return 87;
		}
		BlockState AcaciaLog();
		enum Axis Axis(BlockState Block);
	}
	namespace AcaciaPlanks
	{
		constexpr BlockState AcaciaPlanks()
		{
			return 19;
		}
	}
	namespace AcaciaPressurePlate
	{
		constexpr BlockState AcaciaPressurePlate(const bool Powered)
		{
			if (Powered) return 3881;
			else return 3882;
		}
		BlockState AcaciaPressurePlate();
		bool Powered(BlockState Block);
	}
	namespace AcaciaSapling
	{
		constexpr BlockState AcaciaSapling(const unsigned char Stage)
		{
			if (Stage == 0) return 29;
			else return 30;
		}
		BlockState AcaciaSapling();
		unsigned char Stage(BlockState Block);
	}
	namespace AcaciaSign
	{
		constexpr BlockState AcaciaSign(const unsigned char Rotation)
		{
			if (Rotation == 0) return 3478;
			else if (Rotation == 1) return 3480;
			else if (Rotation == 2) return 3482;
			else if (Rotation == 3) return 3484;
			else if (Rotation == 4) return 3486;
			else if (Rotation == 5) return 3488;
			else if (Rotation == 6) return 3490;
			else if (Rotation == 7) return 3492;
			else if (Rotation == 8) return 3494;
			else if (Rotation == 9) return 3496;
			else if (Rotation == 10) return 3498;
			else if (Rotation == 11) return 3500;
			else if (Rotation == 12) return 3502;
			else if (Rotation == 13) return 3504;
			else if (Rotation == 14) return 3506;
			else return 3508;
		}
		BlockState AcaciaSign();
		unsigned char Rotation(BlockState Block);
	}
	namespace AcaciaSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState AcaciaSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8325;
			else if (Type == Type::Bottom) return 8327;
			else return 8329;
		}
		BlockState AcaciaSlab();
		enum Type Type(BlockState Block);
	}
	namespace AcaciaStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState AcaciaStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7376;
					else if (Shape == Shape::InnerLeft) return 7378;
					else if (Shape == Shape::InnerRight) return 7380;
					else if (Shape == Shape::OuterLeft) return 7382;
					else return 7384;
				else
					if (Shape == Shape::Straight) return 7386;
					else if (Shape == Shape::InnerLeft) return 7388;
					else if (Shape == Shape::InnerRight) return 7390;
					else if (Shape == Shape::OuterLeft) return 7392;
					else return 7394;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7396;
					else if (Shape == Shape::InnerLeft) return 7398;
					else if (Shape == Shape::InnerRight) return 7400;
					else if (Shape == Shape::OuterLeft) return 7402;
					else return 7404;
				else
					if (Shape == Shape::Straight) return 7406;
					else if (Shape == Shape::InnerLeft) return 7408;
					else if (Shape == Shape::InnerRight) return 7410;
					else if (Shape == Shape::OuterLeft) return 7412;
					else return 7414;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7416;
					else if (Shape == Shape::InnerLeft) return 7418;
					else if (Shape == Shape::InnerRight) return 7420;
					else if (Shape == Shape::OuterLeft) return 7422;
					else return 7424;
				else
					if (Shape == Shape::Straight) return 7426;
					else if (Shape == Shape::InnerLeft) return 7428;
					else if (Shape == Shape::InnerRight) return 7430;
					else if (Shape == Shape::OuterLeft) return 7432;
					else return 7434;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7436;
					else if (Shape == Shape::InnerLeft) return 7438;
					else if (Shape == Shape::InnerRight) return 7440;
					else if (Shape == Shape::OuterLeft) return 7442;
					else return 7444;
				else
					if (Shape == Shape::Straight) return 7446;
					else if (Shape == Shape::InnerLeft) return 7448;
					else if (Shape == Shape::InnerRight) return 7450;
					else if (Shape == Shape::OuterLeft) return 7452;
					else return 7454;
		}
		BlockState AcaciaStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace AcaciaTrapdoor
	{
		enum class Half
		{
			Top,
			Bottom
		};
		constexpr BlockState AcaciaTrapdoor(const eBlockFace Facing, const enum Half Half, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4368;
						else return 4370;
					else
						if (Powered) return 4372;
						else return 4374;
				else
					if (Open)
						if (Powered) return 4376;
						else return 4378;
					else
						if (Powered) return 4380;
						else return 4382;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4384;
						else return 4386;
					else
						if (Powered) return 4388;
						else return 4390;
				else
					if (Open)
						if (Powered) return 4392;
						else return 4394;
					else
						if (Powered) return 4396;
						else return 4398;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4400;
						else return 4402;
					else
						if (Powered) return 4404;
						else return 4406;
				else
					if (Open)
						if (Powered) return 4408;
						else return 4410;
					else
						if (Powered) return 4412;
						else return 4414;
			else
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4416;
						else return 4418;
					else
						if (Powered) return 4420;
						else return 4422;
				else
					if (Open)
						if (Powered) return 4424;
						else return 4426;
					else
						if (Powered) return 4428;
						else return 4430;
		}
		BlockState AcaciaTrapdoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace AcaciaWallSign
	{
		constexpr BlockState AcaciaWallSign(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 3760;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 3762;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 3764;
			else return 3766;
		}
		BlockState AcaciaWallSign();
		eBlockFace Facing(BlockState Block);
	}
	namespace AcaciaWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState AcaciaWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 121;
			else if (Axis == Axis::Y) return 122;
			else return 123;
		}
		BlockState AcaciaWood();
		enum Axis Axis(BlockState Block);
	}
	namespace ActivatorRail
	{
		enum class Shape
		{
			NorthSouth,
			EastWest,
			AscendingEast,
			AscendingWest,
			AscendingNorth,
			AscendingSouth
		};
		constexpr BlockState ActivatorRail(const bool Powered, const enum Shape Shape)
		{
			if (Powered)
				if (Shape == Shape::NorthSouth) return 6823;
				else if (Shape == Shape::EastWest) return 6824;
				else if (Shape == Shape::AscendingEast) return 6825;
				else if (Shape == Shape::AscendingWest) return 6826;
				else if (Shape == Shape::AscendingNorth) return 6827;
				else return 6828;
			else
				if (Shape == Shape::NorthSouth) return 6829;
				else if (Shape == Shape::EastWest) return 6830;
				else if (Shape == Shape::AscendingEast) return 6831;
				else if (Shape == Shape::AscendingWest) return 6832;
				else if (Shape == Shape::AscendingNorth) return 6833;
				else return 6834;
		}
		BlockState ActivatorRail();
		bool Powered(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace Air
	{
		constexpr BlockState Air()
		{
			return -0;
		}
	}
	namespace Allium
	{
		constexpr BlockState Allium()
		{
			return 1415;
		}
	}
	namespace AncientDebris
	{
		constexpr BlockState AncientDebris()
		{
			return 15827;
		}
	}
	namespace Andesite
	{
		constexpr BlockState Andesite()
		{
			return 6;
		}
	}
	namespace AndesiteSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState AndesiteSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10844;
			else if (Type == Type::Bottom) return 10846;
			else return 10848;
		}
		BlockState AndesiteSlab();
		enum Type Type(BlockState Block);
	}
	namespace AndesiteStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState AndesiteStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10470;
					else if (Shape == Shape::InnerLeft) return 10472;
					else if (Shape == Shape::InnerRight) return 10474;
					else if (Shape == Shape::OuterLeft) return 10476;
					else return 10478;
				else
					if (Shape == Shape::Straight) return 10480;
					else if (Shape == Shape::InnerLeft) return 10482;
					else if (Shape == Shape::InnerRight) return 10484;
					else if (Shape == Shape::OuterLeft) return 10486;
					else return 10488;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10490;
					else if (Shape == Shape::InnerLeft) return 10492;
					else if (Shape == Shape::InnerRight) return 10494;
					else if (Shape == Shape::OuterLeft) return 10496;
					else return 10498;
				else
					if (Shape == Shape::Straight) return 10500;
					else if (Shape == Shape::InnerLeft) return 10502;
					else if (Shape == Shape::InnerRight) return 10504;
					else if (Shape == Shape::OuterLeft) return 10506;
					else return 10508;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10510;
					else if (Shape == Shape::InnerLeft) return 10512;
					else if (Shape == Shape::InnerRight) return 10514;
					else if (Shape == Shape::OuterLeft) return 10516;
					else return 10518;
				else
					if (Shape == Shape::Straight) return 10520;
					else if (Shape == Shape::InnerLeft) return 10522;
					else if (Shape == Shape::InnerRight) return 10524;
					else if (Shape == Shape::OuterLeft) return 10526;
					else return 10528;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10530;
					else if (Shape == Shape::InnerLeft) return 10532;
					else if (Shape == Shape::InnerRight) return 10534;
					else if (Shape == Shape::OuterLeft) return 10536;
					else return 10538;
				else
					if (Shape == Shape::Straight) return 10540;
					else if (Shape == Shape::InnerLeft) return 10542;
					else if (Shape == Shape::InnerRight) return 10544;
					else if (Shape == Shape::OuterLeft) return 10546;
					else return 10548;
		}
		BlockState AndesiteStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace AndesiteWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState AndesiteWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13138;
							else if (West == West::Low) return 13139;
							else return 13140;
						else
							if (West == West::None) return 13144;
							else if (West == West::Low) return 13145;
							else return 13146;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13150;
							else if (West == West::Low) return 13151;
							else return 13152;
						else
							if (West == West::None) return 13156;
							else if (West == West::Low) return 13157;
							else return 13158;
					else
						if (Up)
							if (West == West::None) return 13162;
							else if (West == West::Low) return 13163;
							else return 13164;
						else
							if (West == West::None) return 13168;
							else if (West == West::Low) return 13169;
							else return 13170;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13174;
							else if (West == West::Low) return 13175;
							else return 13176;
						else
							if (West == West::None) return 13180;
							else if (West == West::Low) return 13181;
							else return 13182;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13186;
							else if (West == West::Low) return 13187;
							else return 13188;
						else
							if (West == West::None) return 13192;
							else if (West == West::Low) return 13193;
							else return 13194;
					else
						if (Up)
							if (West == West::None) return 13198;
							else if (West == West::Low) return 13199;
							else return 13200;
						else
							if (West == West::None) return 13204;
							else if (West == West::Low) return 13205;
							else return 13206;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13210;
							else if (West == West::Low) return 13211;
							else return 13212;
						else
							if (West == West::None) return 13216;
							else if (West == West::Low) return 13217;
							else return 13218;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13222;
							else if (West == West::Low) return 13223;
							else return 13224;
						else
							if (West == West::None) return 13228;
							else if (West == West::Low) return 13229;
							else return 13230;
					else
						if (Up)
							if (West == West::None) return 13234;
							else if (West == West::Low) return 13235;
							else return 13236;
						else
							if (West == West::None) return 13240;
							else if (West == West::Low) return 13241;
							else return 13242;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13246;
							else if (West == West::Low) return 13247;
							else return 13248;
						else
							if (West == West::None) return 13252;
							else if (West == West::Low) return 13253;
							else return 13254;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13258;
							else if (West == West::Low) return 13259;
							else return 13260;
						else
							if (West == West::None) return 13264;
							else if (West == West::Low) return 13265;
							else return 13266;
					else
						if (Up)
							if (West == West::None) return 13270;
							else if (West == West::Low) return 13271;
							else return 13272;
						else
							if (West == West::None) return 13276;
							else if (West == West::Low) return 13277;
							else return 13278;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13282;
							else if (West == West::Low) return 13283;
							else return 13284;
						else
							if (West == West::None) return 13288;
							else if (West == West::Low) return 13289;
							else return 13290;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13294;
							else if (West == West::Low) return 13295;
							else return 13296;
						else
							if (West == West::None) return 13300;
							else if (West == West::Low) return 13301;
							else return 13302;
					else
						if (Up)
							if (West == West::None) return 13306;
							else if (West == West::Low) return 13307;
							else return 13308;
						else
							if (West == West::None) return 13312;
							else if (West == West::Low) return 13313;
							else return 13314;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13318;
							else if (West == West::Low) return 13319;
							else return 13320;
						else
							if (West == West::None) return 13324;
							else if (West == West::Low) return 13325;
							else return 13326;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13330;
							else if (West == West::Low) return 13331;
							else return 13332;
						else
							if (West == West::None) return 13336;
							else if (West == West::Low) return 13337;
							else return 13338;
					else
						if (Up)
							if (West == West::None) return 13342;
							else if (West == West::Low) return 13343;
							else return 13344;
						else
							if (West == West::None) return 13348;
							else if (West == West::Low) return 13349;
							else return 13350;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13354;
							else if (West == West::Low) return 13355;
							else return 13356;
						else
							if (West == West::None) return 13360;
							else if (West == West::Low) return 13361;
							else return 13362;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13366;
							else if (West == West::Low) return 13367;
							else return 13368;
						else
							if (West == West::None) return 13372;
							else if (West == West::Low) return 13373;
							else return 13374;
					else
						if (Up)
							if (West == West::None) return 13378;
							else if (West == West::Low) return 13379;
							else return 13380;
						else
							if (West == West::None) return 13384;
							else if (West == West::Low) return 13385;
							else return 13386;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13390;
							else if (West == West::Low) return 13391;
							else return 13392;
						else
							if (West == West::None) return 13396;
							else if (West == West::Low) return 13397;
							else return 13398;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13402;
							else if (West == West::Low) return 13403;
							else return 13404;
						else
							if (West == West::None) return 13408;
							else if (West == West::Low) return 13409;
							else return 13410;
					else
						if (Up)
							if (West == West::None) return 13414;
							else if (West == West::Low) return 13415;
							else return 13416;
						else
							if (West == West::None) return 13420;
							else if (West == West::Low) return 13421;
							else return 13422;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13426;
							else if (West == West::Low) return 13427;
							else return 13428;
						else
							if (West == West::None) return 13432;
							else if (West == West::Low) return 13433;
							else return 13434;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13438;
							else if (West == West::Low) return 13439;
							else return 13440;
						else
							if (West == West::None) return 13444;
							else if (West == West::Low) return 13445;
							else return 13446;
					else
						if (Up)
							if (West == West::None) return 13450;
							else if (West == West::Low) return 13451;
							else return 13452;
						else
							if (West == West::None) return 13456;
							else if (West == West::Low) return 13457;
							else return 13458;
		}
		BlockState AndesiteWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace Anvil
	{
		constexpr BlockState Anvil(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6610;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6611;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6612;
			else return 6613;
		}
		BlockState Anvil();
		eBlockFace Facing(BlockState Block);
	}
	namespace AttachedMelonStem
	{
		constexpr BlockState AttachedMelonStem(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 4768;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 4769;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 4770;
			else return 4771;
		}
		BlockState AttachedMelonStem();
		eBlockFace Facing(BlockState Block);
	}
	namespace AttachedPumpkinStem
	{
		constexpr BlockState AttachedPumpkinStem(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 4764;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 4765;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 4766;
			else return 4767;
		}
		BlockState AttachedPumpkinStem();
		eBlockFace Facing(BlockState Block);
	}
	namespace AzureBluet
	{
		constexpr BlockState AzureBluet()
		{
			return 1416;
		}
	}
	namespace Bamboo
	{
		enum class Leaves
		{
			None,
			Small,
			Large
		};
		constexpr BlockState Bamboo(const unsigned char Age, const enum Leaves Leaves, const unsigned char Stage)
		{
			if (Age == 0)
				if (Leaves == Leaves::None)
					if (Stage == 0) return 9652;
					else return 9653;
				else if (Leaves == Leaves::Small)
					if (Stage == 0) return 9654;
					else return 9655;
				else
					if (Stage == 0) return 9656;
					else return 9657;
			else
				if (Leaves == Leaves::None)
					if (Stage == 0) return 9658;
					else return 9659;
				else if (Leaves == Leaves::Small)
					if (Stage == 0) return 9660;
					else return 9661;
				else
					if (Stage == 0) return 9662;
					else return 9663;
		}
		BlockState Bamboo();
		unsigned char Age(BlockState Block);
		enum Leaves Leaves(BlockState Block);
		unsigned char Stage(BlockState Block);
	}
	namespace BambooSapling
	{
		constexpr BlockState BambooSapling()
		{
			return 9651;
		}
	}
	namespace Barrel
	{
		constexpr BlockState Barrel(const eBlockFace Facing, const bool Open)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Open) return 14791;
				else return 14792;
			else if (Facing == eBlockFace::BLOCK_FACE_XP)
				if (Open) return 14793;
				else return 14794;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Open) return 14795;
				else return 14796;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Open) return 14797;
				else return 14798;
			else if (Facing == eBlockFace::BLOCK_FACE_YP)
				if (Open) return 14799;
				else return 14800;
			else
				if (Open) return 14801;
				else return 14802;
		}
		BlockState Barrel();
		eBlockFace Facing(BlockState Block);
		bool Open(BlockState Block);
	}
	namespace Barrier
	{
		constexpr BlockState Barrier()
		{
			return 7536;
		}
	}
	namespace Basalt
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState Basalt(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 4002;
			else if (Axis == Axis::Y) return 4003;
			else return 4004;
		}
		BlockState Basalt();
		enum Axis Axis(BlockState Block);
	}
	namespace Beacon
	{
		constexpr BlockState Beacon()
		{
			return 5656;
		}
	}
	namespace Bedrock
	{
		constexpr BlockState Bedrock()
		{
			return 33;
		}
	}
	namespace BeeNest
	{
		constexpr BlockState BeeNest(const eBlockFace Facing, const unsigned char HoneyLevel)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (HoneyLevel == 0) return 15776;
				else if (HoneyLevel == 1) return 15777;
				else if (HoneyLevel == 2) return 15778;
				else if (HoneyLevel == 3) return 15779;
				else if (HoneyLevel == 4) return 15780;
				else return 15781;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (HoneyLevel == 0) return 15782;
				else if (HoneyLevel == 1) return 15783;
				else if (HoneyLevel == 2) return 15784;
				else if (HoneyLevel == 3) return 15785;
				else if (HoneyLevel == 4) return 15786;
				else return 15787;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (HoneyLevel == 0) return 15788;
				else if (HoneyLevel == 1) return 15789;
				else if (HoneyLevel == 2) return 15790;
				else if (HoneyLevel == 3) return 15791;
				else if (HoneyLevel == 4) return 15792;
				else return 15793;
			else
				if (HoneyLevel == 0) return 15794;
				else if (HoneyLevel == 1) return 15795;
				else if (HoneyLevel == 2) return 15796;
				else if (HoneyLevel == 3) return 15797;
				else if (HoneyLevel == 4) return 15798;
				else return 15799;
		}
		BlockState BeeNest();
		eBlockFace Facing(BlockState Block);
		unsigned char HoneyLevel(BlockState Block);
	}
	namespace Beehive
	{
		constexpr BlockState Beehive(const eBlockFace Facing, const unsigned char HoneyLevel)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (HoneyLevel == 0) return 15800;
				else if (HoneyLevel == 1) return 15801;
				else if (HoneyLevel == 2) return 15802;
				else if (HoneyLevel == 3) return 15803;
				else if (HoneyLevel == 4) return 15804;
				else return 15805;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (HoneyLevel == 0) return 15806;
				else if (HoneyLevel == 1) return 15807;
				else if (HoneyLevel == 2) return 15808;
				else if (HoneyLevel == 3) return 15809;
				else if (HoneyLevel == 4) return 15810;
				else return 15811;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (HoneyLevel == 0) return 15812;
				else if (HoneyLevel == 1) return 15813;
				else if (HoneyLevel == 2) return 15814;
				else if (HoneyLevel == 3) return 15815;
				else if (HoneyLevel == 4) return 15816;
				else return 15817;
			else
				if (HoneyLevel == 0) return 15818;
				else if (HoneyLevel == 1) return 15819;
				else if (HoneyLevel == 2) return 15820;
				else if (HoneyLevel == 3) return 15821;
				else if (HoneyLevel == 4) return 15822;
				else return 15823;
		}
		BlockState Beehive();
		eBlockFace Facing(BlockState Block);
		unsigned char HoneyLevel(BlockState Block);
	}
	namespace Beetroots
	{
		constexpr BlockState Beetroots(const unsigned char Age)
		{
			if (Age == 0) return 9219;
			else if (Age == 1) return 9220;
			else if (Age == 2) return 9221;
			else return 9222;
		}
		BlockState Beetroots();
		unsigned char Age(BlockState Block);
	}
	namespace Bell
	{
		enum class Attachment
		{
			Floor,
			Ceiling,
			SingleWall,
			DoubleWall
		};
		constexpr BlockState Bell(const enum Attachment Attachment, const eBlockFace Facing, const bool Powered)
		{
			if (Attachment == Attachment::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 14854;
					else return 14855;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 14856;
					else return 14857;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 14858;
					else return 14859;
				else
					if (Powered) return 14860;
					else return 14861;
			else if (Attachment == Attachment::Ceiling)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 14862;
					else return 14863;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 14864;
					else return 14865;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 14866;
					else return 14867;
				else
					if (Powered) return 14868;
					else return 14869;
			else if (Attachment == Attachment::SingleWall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 14870;
					else return 14871;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 14872;
					else return 14873;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 14874;
					else return 14875;
				else
					if (Powered) return 14876;
					else return 14877;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 14878;
					else return 14879;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 14880;
					else return 14881;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 14882;
					else return 14883;
				else
					if (Powered) return 14884;
					else return 14885;
		}
		BlockState Bell();
		enum Attachment Attachment(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace BirchButton
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState BirchButton(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6394;
					else return 6395;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6396;
					else return 6397;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6398;
					else return 6399;
				else
					if (Powered) return 6400;
					else return 6401;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6402;
					else return 6403;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6404;
					else return 6405;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6406;
					else return 6407;
				else
					if (Powered) return 6408;
					else return 6409;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6410;
					else return 6411;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6412;
					else return 6413;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6414;
					else return 6415;
				else
					if (Powered) return 6416;
					else return 6417;
		}
		BlockState BirchButton();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace BirchDoor
	{
		enum class Half
		{
			Upper,
			Lower
		};
		enum class Hinge
		{
			Left,
			Right
		};
		constexpr BlockState BirchDoor(const eBlockFace Facing, const enum Half Half, const enum Hinge Hinge, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8802;
							else return 8803;
						else
							if (Powered) return 8804;
							else return 8805;
					else
						if (Open)
							if (Powered) return 8806;
							else return 8807;
						else
							if (Powered) return 8808;
							else return 8809;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8810;
							else return 8811;
						else
							if (Powered) return 8812;
							else return 8813;
					else
						if (Open)
							if (Powered) return 8814;
							else return 8815;
						else
							if (Powered) return 8816;
							else return 8817;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8818;
							else return 8819;
						else
							if (Powered) return 8820;
							else return 8821;
					else
						if (Open)
							if (Powered) return 8822;
							else return 8823;
						else
							if (Powered) return 8824;
							else return 8825;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8826;
							else return 8827;
						else
							if (Powered) return 8828;
							else return 8829;
					else
						if (Open)
							if (Powered) return 8830;
							else return 8831;
						else
							if (Powered) return 8832;
							else return 8833;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8834;
							else return 8835;
						else
							if (Powered) return 8836;
							else return 8837;
					else
						if (Open)
							if (Powered) return 8838;
							else return 8839;
						else
							if (Powered) return 8840;
							else return 8841;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8842;
							else return 8843;
						else
							if (Powered) return 8844;
							else return 8845;
					else
						if (Open)
							if (Powered) return 8846;
							else return 8847;
						else
							if (Powered) return 8848;
							else return 8849;
			else
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8850;
							else return 8851;
						else
							if (Powered) return 8852;
							else return 8853;
					else
						if (Open)
							if (Powered) return 8854;
							else return 8855;
						else
							if (Powered) return 8856;
							else return 8857;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8858;
							else return 8859;
						else
							if (Powered) return 8860;
							else return 8861;
					else
						if (Open)
							if (Powered) return 8862;
							else return 8863;
						else
							if (Powered) return 8864;
							else return 8865;
		}
		BlockState BirchDoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Hinge Hinge(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace BirchFence
	{
		constexpr BlockState BirchFence(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 8612;
						else return 8613;
					else
						if (West) return 8616;
						else return 8617;
				else
					if (South)
						if (West) return 8620;
						else return 8621;
					else
						if (West) return 8624;
						else return 8625;
			else
				if (North)
					if (South)
						if (West) return 8628;
						else return 8629;
					else
						if (West) return 8632;
						else return 8633;
				else
					if (South)
						if (West) return 8636;
						else return 8637;
					else
						if (West) return 8640;
						else return 8641;
		}
		BlockState BirchFence();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace BirchFenceGate
	{
		constexpr BlockState BirchFenceGate(const eBlockFace Facing, const bool InWall, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (InWall)
					if (Open)
						if (Powered) return 8450;
						else return 8451;
					else
						if (Powered) return 8452;
						else return 8453;
				else
					if (Open)
						if (Powered) return 8454;
						else return 8455;
					else
						if (Powered) return 8456;
						else return 8457;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (InWall)
					if (Open)
						if (Powered) return 8458;
						else return 8459;
					else
						if (Powered) return 8460;
						else return 8461;
				else
					if (Open)
						if (Powered) return 8462;
						else return 8463;
					else
						if (Powered) return 8464;
						else return 8465;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (InWall)
					if (Open)
						if (Powered) return 8466;
						else return 8467;
					else
						if (Powered) return 8468;
						else return 8469;
				else
					if (Open)
						if (Powered) return 8470;
						else return 8471;
					else
						if (Powered) return 8472;
						else return 8473;
			else
				if (InWall)
					if (Open)
						if (Powered) return 8474;
						else return 8475;
					else
						if (Powered) return 8476;
						else return 8477;
				else
					if (Open)
						if (Powered) return 8478;
						else return 8479;
					else
						if (Powered) return 8480;
						else return 8481;
		}
		BlockState BirchFenceGate();
		eBlockFace Facing(BlockState Block);
		bool InWall(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace BirchLeaves
	{
		constexpr BlockState BirchLeaves(const unsigned char Distance, const bool Persistent)
		{
			if (Distance == 1)
				if (Persistent) return 173;
				else return 174;
			else if (Distance == 2)
				if (Persistent) return 175;
				else return 176;
			else if (Distance == 3)
				if (Persistent) return 177;
				else return 178;
			else if (Distance == 4)
				if (Persistent) return 179;
				else return 180;
			else if (Distance == 5)
				if (Persistent) return 181;
				else return 182;
			else if (Distance == 6)
				if (Persistent) return 183;
				else return 184;
			else
				if (Persistent) return 185;
				else return 186;
		}
		BlockState BirchLeaves();
		unsigned char Distance(BlockState Block);
		bool Persistent(BlockState Block);
	}
	namespace BirchLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState BirchLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 79;
			else if (Axis == Axis::Y) return 80;
			else return 81;
		}
		BlockState BirchLog();
		enum Axis Axis(BlockState Block);
	}
	namespace BirchPlanks
	{
		constexpr BlockState BirchPlanks()
		{
			return 17;
		}
	}
	namespace BirchPressurePlate
	{
		constexpr BlockState BirchPressurePlate(const bool Powered)
		{
			if (Powered) return 3877;
			else return 3878;
		}
		BlockState BirchPressurePlate();
		bool Powered(BlockState Block);
	}
	namespace BirchSapling
	{
		constexpr BlockState BirchSapling(const unsigned char Stage)
		{
			if (Stage == 0) return 25;
			else return 26;
		}
		BlockState BirchSapling();
		unsigned char Stage(BlockState Block);
	}
	namespace BirchSign
	{
		constexpr BlockState BirchSign(const unsigned char Rotation)
		{
			if (Rotation == 0) return 3446;
			else if (Rotation == 1) return 3448;
			else if (Rotation == 2) return 3450;
			else if (Rotation == 3) return 3452;
			else if (Rotation == 4) return 3454;
			else if (Rotation == 5) return 3456;
			else if (Rotation == 6) return 3458;
			else if (Rotation == 7) return 3460;
			else if (Rotation == 8) return 3462;
			else if (Rotation == 9) return 3464;
			else if (Rotation == 10) return 3466;
			else if (Rotation == 11) return 3468;
			else if (Rotation == 12) return 3470;
			else if (Rotation == 13) return 3472;
			else if (Rotation == 14) return 3474;
			else return 3476;
		}
		BlockState BirchSign();
		unsigned char Rotation(BlockState Block);
	}
	namespace BirchSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState BirchSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8313;
			else if (Type == Type::Bottom) return 8315;
			else return 8317;
		}
		BlockState BirchSlab();
		enum Type Type(BlockState Block);
	}
	namespace BirchStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState BirchStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5485;
					else if (Shape == Shape::InnerLeft) return 5487;
					else if (Shape == Shape::InnerRight) return 5489;
					else if (Shape == Shape::OuterLeft) return 5491;
					else return 5493;
				else
					if (Shape == Shape::Straight) return 5495;
					else if (Shape == Shape::InnerLeft) return 5497;
					else if (Shape == Shape::InnerRight) return 5499;
					else if (Shape == Shape::OuterLeft) return 5501;
					else return 5503;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5505;
					else if (Shape == Shape::InnerLeft) return 5507;
					else if (Shape == Shape::InnerRight) return 5509;
					else if (Shape == Shape::OuterLeft) return 5511;
					else return 5513;
				else
					if (Shape == Shape::Straight) return 5515;
					else if (Shape == Shape::InnerLeft) return 5517;
					else if (Shape == Shape::InnerRight) return 5519;
					else if (Shape == Shape::OuterLeft) return 5521;
					else return 5523;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5525;
					else if (Shape == Shape::InnerLeft) return 5527;
					else if (Shape == Shape::InnerRight) return 5529;
					else if (Shape == Shape::OuterLeft) return 5531;
					else return 5533;
				else
					if (Shape == Shape::Straight) return 5535;
					else if (Shape == Shape::InnerLeft) return 5537;
					else if (Shape == Shape::InnerRight) return 5539;
					else if (Shape == Shape::OuterLeft) return 5541;
					else return 5543;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5545;
					else if (Shape == Shape::InnerLeft) return 5547;
					else if (Shape == Shape::InnerRight) return 5549;
					else if (Shape == Shape::OuterLeft) return 5551;
					else return 5553;
				else
					if (Shape == Shape::Straight) return 5555;
					else if (Shape == Shape::InnerLeft) return 5557;
					else if (Shape == Shape::InnerRight) return 5559;
					else if (Shape == Shape::OuterLeft) return 5561;
					else return 5563;
		}
		BlockState BirchStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace BirchTrapdoor
	{
		enum class Half
		{
			Top,
			Bottom
		};
		constexpr BlockState BirchTrapdoor(const eBlockFace Facing, const enum Half Half, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4240;
						else return 4242;
					else
						if (Powered) return 4244;
						else return 4246;
				else
					if (Open)
						if (Powered) return 4248;
						else return 4250;
					else
						if (Powered) return 4252;
						else return 4254;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4256;
						else return 4258;
					else
						if (Powered) return 4260;
						else return 4262;
				else
					if (Open)
						if (Powered) return 4264;
						else return 4266;
					else
						if (Powered) return 4268;
						else return 4270;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4272;
						else return 4274;
					else
						if (Powered) return 4276;
						else return 4278;
				else
					if (Open)
						if (Powered) return 4280;
						else return 4282;
					else
						if (Powered) return 4284;
						else return 4286;
			else
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4288;
						else return 4290;
					else
						if (Powered) return 4292;
						else return 4294;
				else
					if (Open)
						if (Powered) return 4296;
						else return 4298;
					else
						if (Powered) return 4300;
						else return 4302;
		}
		BlockState BirchTrapdoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace BirchWallSign
	{
		constexpr BlockState BirchWallSign(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 3752;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 3754;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 3756;
			else return 3758;
		}
		BlockState BirchWallSign();
		eBlockFace Facing(BlockState Block);
	}
	namespace BirchWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState BirchWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 115;
			else if (Axis == Axis::Y) return 116;
			else return 117;
		}
		BlockState BirchWood();
		enum Axis Axis(BlockState Block);
	}
	namespace BlackBanner
	{
		constexpr BlockState BlackBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 8137;
			else if (Rotation == 1) return 8138;
			else if (Rotation == 2) return 8139;
			else if (Rotation == 3) return 8140;
			else if (Rotation == 4) return 8141;
			else if (Rotation == 5) return 8142;
			else if (Rotation == 6) return 8143;
			else if (Rotation == 7) return 8144;
			else if (Rotation == 8) return 8145;
			else if (Rotation == 9) return 8146;
			else if (Rotation == 10) return 8147;
			else if (Rotation == 11) return 8148;
			else if (Rotation == 12) return 8149;
			else if (Rotation == 13) return 8150;
			else if (Rotation == 14) return 8151;
			else return 8152;
		}
		BlockState BlackBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace BlackBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState BlackBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1289;
					else return 1290;
				else
					if (Part == Part::Head) return 1291;
					else return 1292;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1293;
					else return 1294;
				else
					if (Part == Part::Head) return 1295;
					else return 1296;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1297;
					else return 1298;
				else
					if (Part == Part::Head) return 1299;
					else return 1300;
			else
				if (Occupied)
					if (Part == Part::Head) return 1301;
					else return 1302;
				else
					if (Part == Part::Head) return 1303;
					else return 1304;
		}
		BlockState BlackBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace BlackCarpet
	{
		constexpr BlockState BlackCarpet()
		{
			return 7881;
		}
	}
	namespace BlackConcrete
	{
		constexpr BlockState BlackConcrete()
		{
			return 9453;
		}
	}
	namespace BlackConcretePowder
	{
		constexpr BlockState BlackConcretePowder()
		{
			return 9469;
		}
	}
	namespace BlackGlazedTerracotta
	{
		constexpr BlockState BlackGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9434;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9435;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9436;
			else return 9437;
		}
		BlockState BlackGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace BlackShulkerBox
	{
		constexpr BlockState BlackShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9368;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9369;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9370;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9371;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9372;
			else return 9373;
		}
		BlockState BlackShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace BlackStainedGlass
	{
		constexpr BlockState BlackStainedGlass()
		{
			return 4110;
		}
	}
	namespace BlackStainedGlassPane
	{
		constexpr BlockState BlackStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7345;
						else return 7346;
					else
						if (West) return 7349;
						else return 7350;
				else
					if (South)
						if (West) return 7353;
						else return 7354;
					else
						if (West) return 7357;
						else return 7358;
			else
				if (North)
					if (South)
						if (West) return 7361;
						else return 7362;
					else
						if (West) return 7365;
						else return 7366;
				else
					if (South)
						if (West) return 7369;
						else return 7370;
					else
						if (West) return 7373;
						else return 7374;
		}
		BlockState BlackStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace BlackTerracotta
	{
		constexpr BlockState BlackTerracotta()
		{
			return 6862;
		}
	}
	namespace BlackWallBanner
	{
		constexpr BlockState BlackWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8213;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8214;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8215;
			else return 8216;
		}
		BlockState BlackWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace BlackWool
	{
		constexpr BlockState BlackWool()
		{
			return 1399;
		}
	}
	namespace Blackstone
	{
		constexpr BlockState Blackstone()
		{
			return 15839;
		}
	}
	namespace BlackstoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState BlackstoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 16245;
			else if (Type == Type::Bottom) return 16247;
			else return 16249;
		}
		BlockState BlackstoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace BlackstoneStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState BlackstoneStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15841;
					else if (Shape == Shape::InnerLeft) return 15843;
					else if (Shape == Shape::InnerRight) return 15845;
					else if (Shape == Shape::OuterLeft) return 15847;
					else return 15849;
				else
					if (Shape == Shape::Straight) return 15851;
					else if (Shape == Shape::InnerLeft) return 15853;
					else if (Shape == Shape::InnerRight) return 15855;
					else if (Shape == Shape::OuterLeft) return 15857;
					else return 15859;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15861;
					else if (Shape == Shape::InnerLeft) return 15863;
					else if (Shape == Shape::InnerRight) return 15865;
					else if (Shape == Shape::OuterLeft) return 15867;
					else return 15869;
				else
					if (Shape == Shape::Straight) return 15871;
					else if (Shape == Shape::InnerLeft) return 15873;
					else if (Shape == Shape::InnerRight) return 15875;
					else if (Shape == Shape::OuterLeft) return 15877;
					else return 15879;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15881;
					else if (Shape == Shape::InnerLeft) return 15883;
					else if (Shape == Shape::InnerRight) return 15885;
					else if (Shape == Shape::OuterLeft) return 15887;
					else return 15889;
				else
					if (Shape == Shape::Straight) return 15891;
					else if (Shape == Shape::InnerLeft) return 15893;
					else if (Shape == Shape::InnerRight) return 15895;
					else if (Shape == Shape::OuterLeft) return 15897;
					else return 15899;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15901;
					else if (Shape == Shape::InnerLeft) return 15903;
					else if (Shape == Shape::InnerRight) return 15905;
					else if (Shape == Shape::OuterLeft) return 15907;
					else return 15909;
				else
					if (Shape == Shape::Straight) return 15911;
					else if (Shape == Shape::InnerLeft) return 15913;
					else if (Shape == Shape::InnerRight) return 15915;
					else if (Shape == Shape::OuterLeft) return 15917;
					else return 15919;
		}
		BlockState BlackstoneStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace BlackstoneWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState BlackstoneWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 15923;
							else if (West == West::Low) return 15924;
							else return 15925;
						else
							if (West == West::None) return 15929;
							else if (West == West::Low) return 15930;
							else return 15931;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 15935;
							else if (West == West::Low) return 15936;
							else return 15937;
						else
							if (West == West::None) return 15941;
							else if (West == West::Low) return 15942;
							else return 15943;
					else
						if (Up)
							if (West == West::None) return 15947;
							else if (West == West::Low) return 15948;
							else return 15949;
						else
							if (West == West::None) return 15953;
							else if (West == West::Low) return 15954;
							else return 15955;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 15959;
							else if (West == West::Low) return 15960;
							else return 15961;
						else
							if (West == West::None) return 15965;
							else if (West == West::Low) return 15966;
							else return 15967;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 15971;
							else if (West == West::Low) return 15972;
							else return 15973;
						else
							if (West == West::None) return 15977;
							else if (West == West::Low) return 15978;
							else return 15979;
					else
						if (Up)
							if (West == West::None) return 15983;
							else if (West == West::Low) return 15984;
							else return 15985;
						else
							if (West == West::None) return 15989;
							else if (West == West::Low) return 15990;
							else return 15991;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 15995;
							else if (West == West::Low) return 15996;
							else return 15997;
						else
							if (West == West::None) return 16001;
							else if (West == West::Low) return 16002;
							else return 16003;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16007;
							else if (West == West::Low) return 16008;
							else return 16009;
						else
							if (West == West::None) return 16013;
							else if (West == West::Low) return 16014;
							else return 16015;
					else
						if (Up)
							if (West == West::None) return 16019;
							else if (West == West::Low) return 16020;
							else return 16021;
						else
							if (West == West::None) return 16025;
							else if (West == West::Low) return 16026;
							else return 16027;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16031;
							else if (West == West::Low) return 16032;
							else return 16033;
						else
							if (West == West::None) return 16037;
							else if (West == West::Low) return 16038;
							else return 16039;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16043;
							else if (West == West::Low) return 16044;
							else return 16045;
						else
							if (West == West::None) return 16049;
							else if (West == West::Low) return 16050;
							else return 16051;
					else
						if (Up)
							if (West == West::None) return 16055;
							else if (West == West::Low) return 16056;
							else return 16057;
						else
							if (West == West::None) return 16061;
							else if (West == West::Low) return 16062;
							else return 16063;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16067;
							else if (West == West::Low) return 16068;
							else return 16069;
						else
							if (West == West::None) return 16073;
							else if (West == West::Low) return 16074;
							else return 16075;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16079;
							else if (West == West::Low) return 16080;
							else return 16081;
						else
							if (West == West::None) return 16085;
							else if (West == West::Low) return 16086;
							else return 16087;
					else
						if (Up)
							if (West == West::None) return 16091;
							else if (West == West::Low) return 16092;
							else return 16093;
						else
							if (West == West::None) return 16097;
							else if (West == West::Low) return 16098;
							else return 16099;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16103;
							else if (West == West::Low) return 16104;
							else return 16105;
						else
							if (West == West::None) return 16109;
							else if (West == West::Low) return 16110;
							else return 16111;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16115;
							else if (West == West::Low) return 16116;
							else return 16117;
						else
							if (West == West::None) return 16121;
							else if (West == West::Low) return 16122;
							else return 16123;
					else
						if (Up)
							if (West == West::None) return 16127;
							else if (West == West::Low) return 16128;
							else return 16129;
						else
							if (West == West::None) return 16133;
							else if (West == West::Low) return 16134;
							else return 16135;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16139;
							else if (West == West::Low) return 16140;
							else return 16141;
						else
							if (West == West::None) return 16145;
							else if (West == West::Low) return 16146;
							else return 16147;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16151;
							else if (West == West::Low) return 16152;
							else return 16153;
						else
							if (West == West::None) return 16157;
							else if (West == West::Low) return 16158;
							else return 16159;
					else
						if (Up)
							if (West == West::None) return 16163;
							else if (West == West::Low) return 16164;
							else return 16165;
						else
							if (West == West::None) return 16169;
							else if (West == West::Low) return 16170;
							else return 16171;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16175;
							else if (West == West::Low) return 16176;
							else return 16177;
						else
							if (West == West::None) return 16181;
							else if (West == West::Low) return 16182;
							else return 16183;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16187;
							else if (West == West::Low) return 16188;
							else return 16189;
						else
							if (West == West::None) return 16193;
							else if (West == West::Low) return 16194;
							else return 16195;
					else
						if (Up)
							if (West == West::None) return 16199;
							else if (West == West::Low) return 16200;
							else return 16201;
						else
							if (West == West::None) return 16205;
							else if (West == West::Low) return 16206;
							else return 16207;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16211;
							else if (West == West::Low) return 16212;
							else return 16213;
						else
							if (West == West::None) return 16217;
							else if (West == West::Low) return 16218;
							else return 16219;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16223;
							else if (West == West::Low) return 16224;
							else return 16225;
						else
							if (West == West::None) return 16229;
							else if (West == West::Low) return 16230;
							else return 16231;
					else
						if (Up)
							if (West == West::None) return 16235;
							else if (West == West::Low) return 16236;
							else return 16237;
						else
							if (West == West::None) return 16241;
							else if (West == West::Low) return 16242;
							else return 16243;
		}
		BlockState BlackstoneWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace BlastFurnace
	{
		constexpr BlockState BlastFurnace(const eBlockFace Facing, const bool Lit)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Lit) return 14811;
				else return 14812;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Lit) return 14813;
				else return 14814;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Lit) return 14815;
				else return 14816;
			else
				if (Lit) return 14817;
				else return 14818;
		}
		BlockState BlastFurnace();
		eBlockFace Facing(BlockState Block);
		bool Lit(BlockState Block);
	}
	namespace BlueBanner
	{
		constexpr BlockState BlueBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 8073;
			else if (Rotation == 1) return 8074;
			else if (Rotation == 2) return 8075;
			else if (Rotation == 3) return 8076;
			else if (Rotation == 4) return 8077;
			else if (Rotation == 5) return 8078;
			else if (Rotation == 6) return 8079;
			else if (Rotation == 7) return 8080;
			else if (Rotation == 8) return 8081;
			else if (Rotation == 9) return 8082;
			else if (Rotation == 10) return 8083;
			else if (Rotation == 11) return 8084;
			else if (Rotation == 12) return 8085;
			else if (Rotation == 13) return 8086;
			else if (Rotation == 14) return 8087;
			else return 8088;
		}
		BlockState BlueBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace BlueBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState BlueBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1225;
					else return 1226;
				else
					if (Part == Part::Head) return 1227;
					else return 1228;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1229;
					else return 1230;
				else
					if (Part == Part::Head) return 1231;
					else return 1232;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1233;
					else return 1234;
				else
					if (Part == Part::Head) return 1235;
					else return 1236;
			else
				if (Occupied)
					if (Part == Part::Head) return 1237;
					else return 1238;
				else
					if (Part == Part::Head) return 1239;
					else return 1240;
		}
		BlockState BlueBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace BlueCarpet
	{
		constexpr BlockState BlueCarpet()
		{
			return 7877;
		}
	}
	namespace BlueConcrete
	{
		constexpr BlockState BlueConcrete()
		{
			return 9449;
		}
	}
	namespace BlueConcretePowder
	{
		constexpr BlockState BlueConcretePowder()
		{
			return 9465;
		}
	}
	namespace BlueGlazedTerracotta
	{
		constexpr BlockState BlueGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9418;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9419;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9420;
			else return 9421;
		}
		BlockState BlueGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace BlueIce
	{
		constexpr BlockState BlueIce()
		{
			return 9648;
		}
	}
	namespace BlueOrchid
	{
		constexpr BlockState BlueOrchid()
		{
			return 1414;
		}
	}
	namespace BlueShulkerBox
	{
		constexpr BlockState BlueShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9344;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9345;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9346;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9347;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9348;
			else return 9349;
		}
		BlockState BlueShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace BlueStainedGlass
	{
		constexpr BlockState BlueStainedGlass()
		{
			return 4106;
		}
	}
	namespace BlueStainedGlassPane
	{
		constexpr BlockState BlueStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7217;
						else return 7218;
					else
						if (West) return 7221;
						else return 7222;
				else
					if (South)
						if (West) return 7225;
						else return 7226;
					else
						if (West) return 7229;
						else return 7230;
			else
				if (North)
					if (South)
						if (West) return 7233;
						else return 7234;
					else
						if (West) return 7237;
						else return 7238;
				else
					if (South)
						if (West) return 7241;
						else return 7242;
					else
						if (West) return 7245;
						else return 7246;
		}
		BlockState BlueStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace BlueTerracotta
	{
		constexpr BlockState BlueTerracotta()
		{
			return 6858;
		}
	}
	namespace BlueWallBanner
	{
		constexpr BlockState BlueWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8197;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8198;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8199;
			else return 8200;
		}
		BlockState BlueWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace BlueWool
	{
		constexpr BlockState BlueWool()
		{
			return 1395;
		}
	}
	namespace BoneBlock
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState BoneBlock(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 9256;
			else if (Axis == Axis::Y) return 9257;
			else return 9258;
		}
		BlockState BoneBlock();
		enum Axis Axis(BlockState Block);
	}
	namespace Bookshelf
	{
		constexpr BlockState Bookshelf()
		{
			return 1432;
		}
	}
	namespace BrainCoral
	{
		constexpr BlockState BrainCoral()
		{
			return 9533;
		}
	}
	namespace BrainCoralBlock
	{
		constexpr BlockState BrainCoralBlock()
		{
			return 9516;
		}
	}
	namespace BrainCoralFan
	{
		constexpr BlockState BrainCoralFan()
		{
			return 9553;
		}
	}
	namespace BrainCoralWallFan
	{
		constexpr BlockState BrainCoralWallFan(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9609;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9611;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9613;
			else return 9615;
		}
		BlockState BrainCoralWallFan();
		eBlockFace Facing(BlockState Block);
	}
	namespace BrewingStand
	{
		constexpr BlockState BrewingStand(const bool HasBottle_0, const bool HasBottle_1, const bool HasBottle_2)
		{
			if (HasBottle_0)
				if (HasBottle_1)
					if (HasBottle_2) return 5133;
					else return 5134;
				else
					if (HasBottle_2) return 5135;
					else return 5136;
			else
				if (HasBottle_1)
					if (HasBottle_2) return 5137;
					else return 5138;
				else
					if (HasBottle_2) return 5139;
					else return 5140;
		}
		BlockState BrewingStand();
		bool HasBottle_0(BlockState Block);
		bool HasBottle_1(BlockState Block);
		bool HasBottle_2(BlockState Block);
	}
	namespace BrickSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState BrickSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8373;
			else if (Type == Type::Bottom) return 8375;
			else return 8377;
		}
		BlockState BrickSlab();
		enum Type Type(BlockState Block);
	}
	namespace BrickStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState BrickStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 4853;
					else if (Shape == Shape::InnerLeft) return 4855;
					else if (Shape == Shape::InnerRight) return 4857;
					else if (Shape == Shape::OuterLeft) return 4859;
					else return 4861;
				else
					if (Shape == Shape::Straight) return 4863;
					else if (Shape == Shape::InnerLeft) return 4865;
					else if (Shape == Shape::InnerRight) return 4867;
					else if (Shape == Shape::OuterLeft) return 4869;
					else return 4871;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 4873;
					else if (Shape == Shape::InnerLeft) return 4875;
					else if (Shape == Shape::InnerRight) return 4877;
					else if (Shape == Shape::OuterLeft) return 4879;
					else return 4881;
				else
					if (Shape == Shape::Straight) return 4883;
					else if (Shape == Shape::InnerLeft) return 4885;
					else if (Shape == Shape::InnerRight) return 4887;
					else if (Shape == Shape::OuterLeft) return 4889;
					else return 4891;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 4893;
					else if (Shape == Shape::InnerLeft) return 4895;
					else if (Shape == Shape::InnerRight) return 4897;
					else if (Shape == Shape::OuterLeft) return 4899;
					else return 4901;
				else
					if (Shape == Shape::Straight) return 4903;
					else if (Shape == Shape::InnerLeft) return 4905;
					else if (Shape == Shape::InnerRight) return 4907;
					else if (Shape == Shape::OuterLeft) return 4909;
					else return 4911;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 4913;
					else if (Shape == Shape::InnerLeft) return 4915;
					else if (Shape == Shape::InnerRight) return 4917;
					else if (Shape == Shape::OuterLeft) return 4919;
					else return 4921;
				else
					if (Shape == Shape::Straight) return 4923;
					else if (Shape == Shape::InnerLeft) return 4925;
					else if (Shape == Shape::InnerRight) return 4927;
					else if (Shape == Shape::OuterLeft) return 4929;
					else return 4931;
		}
		BlockState BrickStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace BrickWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState BrickWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 10870;
							else if (West == West::Low) return 10871;
							else return 10872;
						else
							if (West == West::None) return 10876;
							else if (West == West::Low) return 10877;
							else return 10878;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 10882;
							else if (West == West::Low) return 10883;
							else return 10884;
						else
							if (West == West::None) return 10888;
							else if (West == West::Low) return 10889;
							else return 10890;
					else
						if (Up)
							if (West == West::None) return 10894;
							else if (West == West::Low) return 10895;
							else return 10896;
						else
							if (West == West::None) return 10900;
							else if (West == West::Low) return 10901;
							else return 10902;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 10906;
							else if (West == West::Low) return 10907;
							else return 10908;
						else
							if (West == West::None) return 10912;
							else if (West == West::Low) return 10913;
							else return 10914;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 10918;
							else if (West == West::Low) return 10919;
							else return 10920;
						else
							if (West == West::None) return 10924;
							else if (West == West::Low) return 10925;
							else return 10926;
					else
						if (Up)
							if (West == West::None) return 10930;
							else if (West == West::Low) return 10931;
							else return 10932;
						else
							if (West == West::None) return 10936;
							else if (West == West::Low) return 10937;
							else return 10938;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 10942;
							else if (West == West::Low) return 10943;
							else return 10944;
						else
							if (West == West::None) return 10948;
							else if (West == West::Low) return 10949;
							else return 10950;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 10954;
							else if (West == West::Low) return 10955;
							else return 10956;
						else
							if (West == West::None) return 10960;
							else if (West == West::Low) return 10961;
							else return 10962;
					else
						if (Up)
							if (West == West::None) return 10966;
							else if (West == West::Low) return 10967;
							else return 10968;
						else
							if (West == West::None) return 10972;
							else if (West == West::Low) return 10973;
							else return 10974;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 10978;
							else if (West == West::Low) return 10979;
							else return 10980;
						else
							if (West == West::None) return 10984;
							else if (West == West::Low) return 10985;
							else return 10986;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 10990;
							else if (West == West::Low) return 10991;
							else return 10992;
						else
							if (West == West::None) return 10996;
							else if (West == West::Low) return 10997;
							else return 10998;
					else
						if (Up)
							if (West == West::None) return 11002;
							else if (West == West::Low) return 11003;
							else return 11004;
						else
							if (West == West::None) return 11008;
							else if (West == West::Low) return 11009;
							else return 11010;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11014;
							else if (West == West::Low) return 11015;
							else return 11016;
						else
							if (West == West::None) return 11020;
							else if (West == West::Low) return 11021;
							else return 11022;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11026;
							else if (West == West::Low) return 11027;
							else return 11028;
						else
							if (West == West::None) return 11032;
							else if (West == West::Low) return 11033;
							else return 11034;
					else
						if (Up)
							if (West == West::None) return 11038;
							else if (West == West::Low) return 11039;
							else return 11040;
						else
							if (West == West::None) return 11044;
							else if (West == West::Low) return 11045;
							else return 11046;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11050;
							else if (West == West::Low) return 11051;
							else return 11052;
						else
							if (West == West::None) return 11056;
							else if (West == West::Low) return 11057;
							else return 11058;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11062;
							else if (West == West::Low) return 11063;
							else return 11064;
						else
							if (West == West::None) return 11068;
							else if (West == West::Low) return 11069;
							else return 11070;
					else
						if (Up)
							if (West == West::None) return 11074;
							else if (West == West::Low) return 11075;
							else return 11076;
						else
							if (West == West::None) return 11080;
							else if (West == West::Low) return 11081;
							else return 11082;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11086;
							else if (West == West::Low) return 11087;
							else return 11088;
						else
							if (West == West::None) return 11092;
							else if (West == West::Low) return 11093;
							else return 11094;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11098;
							else if (West == West::Low) return 11099;
							else return 11100;
						else
							if (West == West::None) return 11104;
							else if (West == West::Low) return 11105;
							else return 11106;
					else
						if (Up)
							if (West == West::None) return 11110;
							else if (West == West::Low) return 11111;
							else return 11112;
						else
							if (West == West::None) return 11116;
							else if (West == West::Low) return 11117;
							else return 11118;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11122;
							else if (West == West::Low) return 11123;
							else return 11124;
						else
							if (West == West::None) return 11128;
							else if (West == West::Low) return 11129;
							else return 11130;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11134;
							else if (West == West::Low) return 11135;
							else return 11136;
						else
							if (West == West::None) return 11140;
							else if (West == West::Low) return 11141;
							else return 11142;
					else
						if (Up)
							if (West == West::None) return 11146;
							else if (West == West::Low) return 11147;
							else return 11148;
						else
							if (West == West::None) return 11152;
							else if (West == West::Low) return 11153;
							else return 11154;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11158;
							else if (West == West::Low) return 11159;
							else return 11160;
						else
							if (West == West::None) return 11164;
							else if (West == West::Low) return 11165;
							else return 11166;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11170;
							else if (West == West::Low) return 11171;
							else return 11172;
						else
							if (West == West::None) return 11176;
							else if (West == West::Low) return 11177;
							else return 11178;
					else
						if (Up)
							if (West == West::None) return 11182;
							else if (West == West::Low) return 11183;
							else return 11184;
						else
							if (West == West::None) return 11188;
							else if (West == West::Low) return 11189;
							else return 11190;
		}
		BlockState BrickWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace Bricks
	{
		constexpr BlockState Bricks()
		{
			return 1429;
		}
	}
	namespace BrownBanner
	{
		constexpr BlockState BrownBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 8089;
			else if (Rotation == 1) return 8090;
			else if (Rotation == 2) return 8091;
			else if (Rotation == 3) return 8092;
			else if (Rotation == 4) return 8093;
			else if (Rotation == 5) return 8094;
			else if (Rotation == 6) return 8095;
			else if (Rotation == 7) return 8096;
			else if (Rotation == 8) return 8097;
			else if (Rotation == 9) return 8098;
			else if (Rotation == 10) return 8099;
			else if (Rotation == 11) return 8100;
			else if (Rotation == 12) return 8101;
			else if (Rotation == 13) return 8102;
			else if (Rotation == 14) return 8103;
			else return 8104;
		}
		BlockState BrownBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace BrownBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState BrownBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1241;
					else return 1242;
				else
					if (Part == Part::Head) return 1243;
					else return 1244;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1245;
					else return 1246;
				else
					if (Part == Part::Head) return 1247;
					else return 1248;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1249;
					else return 1250;
				else
					if (Part == Part::Head) return 1251;
					else return 1252;
			else
				if (Occupied)
					if (Part == Part::Head) return 1253;
					else return 1254;
				else
					if (Part == Part::Head) return 1255;
					else return 1256;
		}
		BlockState BrownBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace BrownCarpet
	{
		constexpr BlockState BrownCarpet()
		{
			return 7878;
		}
	}
	namespace BrownConcrete
	{
		constexpr BlockState BrownConcrete()
		{
			return 9450;
		}
	}
	namespace BrownConcretePowder
	{
		constexpr BlockState BrownConcretePowder()
		{
			return 9466;
		}
	}
	namespace BrownGlazedTerracotta
	{
		constexpr BlockState BrownGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9422;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9423;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9424;
			else return 9425;
		}
		BlockState BrownGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace BrownMushroom
	{
		constexpr BlockState BrownMushroom()
		{
			return 1425;
		}
	}
	namespace BrownMushroomBlock
	{
		constexpr BlockState BrownMushroomBlock(const bool Down, const bool East, const bool North, const bool South, const bool Up, const bool West)
		{
			if (Down)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 4505;
								else return 4506;
							else
								if (West) return 4507;
								else return 4508;
						else
							if (Up)
								if (West) return 4509;
								else return 4510;
							else
								if (West) return 4511;
								else return 4512;
					else
						if (South)
							if (Up)
								if (West) return 4513;
								else return 4514;
							else
								if (West) return 4515;
								else return 4516;
						else
							if (Up)
								if (West) return 4517;
								else return 4518;
							else
								if (West) return 4519;
								else return 4520;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 4521;
								else return 4522;
							else
								if (West) return 4523;
								else return 4524;
						else
							if (Up)
								if (West) return 4525;
								else return 4526;
							else
								if (West) return 4527;
								else return 4528;
					else
						if (South)
							if (Up)
								if (West) return 4529;
								else return 4530;
							else
								if (West) return 4531;
								else return 4532;
						else
							if (Up)
								if (West) return 4533;
								else return 4534;
							else
								if (West) return 4535;
								else return 4536;
			else
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 4537;
								else return 4538;
							else
								if (West) return 4539;
								else return 4540;
						else
							if (Up)
								if (West) return 4541;
								else return 4542;
							else
								if (West) return 4543;
								else return 4544;
					else
						if (South)
							if (Up)
								if (West) return 4545;
								else return 4546;
							else
								if (West) return 4547;
								else return 4548;
						else
							if (Up)
								if (West) return 4549;
								else return 4550;
							else
								if (West) return 4551;
								else return 4552;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 4553;
								else return 4554;
							else
								if (West) return 4555;
								else return 4556;
						else
							if (Up)
								if (West) return 4557;
								else return 4558;
							else
								if (West) return 4559;
								else return 4560;
					else
						if (South)
							if (Up)
								if (West) return 4561;
								else return 4562;
							else
								if (West) return 4563;
								else return 4564;
						else
							if (Up)
								if (West) return 4565;
								else return 4566;
							else
								if (West) return 4567;
								else return 4568;
		}
		BlockState BrownMushroomBlock();
		bool Down(BlockState Block);
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool Up(BlockState Block);
		bool West(BlockState Block);
	}
	namespace BrownShulkerBox
	{
		constexpr BlockState BrownShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9350;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9351;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9352;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9353;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9354;
			else return 9355;
		}
		BlockState BrownShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace BrownStainedGlass
	{
		constexpr BlockState BrownStainedGlass()
		{
			return 4107;
		}
	}
	namespace BrownStainedGlassPane
	{
		constexpr BlockState BrownStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7249;
						else return 7250;
					else
						if (West) return 7253;
						else return 7254;
				else
					if (South)
						if (West) return 7257;
						else return 7258;
					else
						if (West) return 7261;
						else return 7262;
			else
				if (North)
					if (South)
						if (West) return 7265;
						else return 7266;
					else
						if (West) return 7269;
						else return 7270;
				else
					if (South)
						if (West) return 7273;
						else return 7274;
					else
						if (West) return 7277;
						else return 7278;
		}
		BlockState BrownStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace BrownTerracotta
	{
		constexpr BlockState BrownTerracotta()
		{
			return 6859;
		}
	}
	namespace BrownWallBanner
	{
		constexpr BlockState BrownWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8201;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8202;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8203;
			else return 8204;
		}
		BlockState BrownWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace BrownWool
	{
		constexpr BlockState BrownWool()
		{
			return 1396;
		}
	}
	namespace BubbleColumn
	{
		constexpr BlockState BubbleColumn(const bool Drag)
		{
			if (Drag) return 9667;
			else return 9668;
		}
		BlockState BubbleColumn();
		bool Drag(BlockState Block);
	}
	namespace BubbleCoral
	{
		constexpr BlockState BubbleCoral()
		{
			return 9535;
		}
	}
	namespace BubbleCoralBlock
	{
		constexpr BlockState BubbleCoralBlock()
		{
			return 9517;
		}
	}
	namespace BubbleCoralFan
	{
		constexpr BlockState BubbleCoralFan()
		{
			return 9555;
		}
	}
	namespace BubbleCoralWallFan
	{
		constexpr BlockState BubbleCoralWallFan(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9617;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9619;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9621;
			else return 9623;
		}
		BlockState BubbleCoralWallFan();
		eBlockFace Facing(BlockState Block);
	}
	namespace Cactus
	{
		constexpr BlockState Cactus(const unsigned char Age)
		{
			if (Age == 0) return 3931;
			else if (Age == 1) return 3932;
			else if (Age == 2) return 3933;
			else if (Age == 3) return 3934;
			else if (Age == 4) return 3935;
			else if (Age == 5) return 3936;
			else if (Age == 6) return 3937;
			else if (Age == 7) return 3938;
			else if (Age == 8) return 3939;
			else if (Age == 9) return 3940;
			else if (Age == 10) return 3941;
			else if (Age == 11) return 3942;
			else if (Age == 12) return 3943;
			else if (Age == 13) return 3944;
			else if (Age == 14) return 3945;
			else return 3946;
		}
		BlockState Cactus();
		unsigned char Age(BlockState Block);
	}
	namespace Cake
	{
		constexpr BlockState Cake(const unsigned char Bites)
		{
			if (Bites == 0) return 4024;
			else if (Bites == 1) return 4025;
			else if (Bites == 2) return 4026;
			else if (Bites == 3) return 4027;
			else if (Bites == 4) return 4028;
			else if (Bites == 5) return 4029;
			else return 4030;
		}
		BlockState Cake();
		unsigned char Bites(BlockState Block);
	}
	namespace Campfire
	{
		constexpr BlockState Campfire(const eBlockFace Facing, const bool Lit, const bool SignalFire)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Lit)
					if (SignalFire) return 14891;
					else return 14893;
				else
					if (SignalFire) return 14895;
					else return 14897;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Lit)
					if (SignalFire) return 14899;
					else return 14901;
				else
					if (SignalFire) return 14903;
					else return 14905;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Lit)
					if (SignalFire) return 14907;
					else return 14909;
				else
					if (SignalFire) return 14911;
					else return 14913;
			else
				if (Lit)
					if (SignalFire) return 14915;
					else return 14917;
				else
					if (SignalFire) return 14919;
					else return 14921;
		}
		BlockState Campfire();
		eBlockFace Facing(BlockState Block);
		bool Lit(BlockState Block);
		bool SignalFire(BlockState Block);
	}
	namespace Carrots
	{
		constexpr BlockState Carrots(const unsigned char Age)
		{
			if (Age == 0) return 6330;
			else if (Age == 1) return 6331;
			else if (Age == 2) return 6332;
			else if (Age == 3) return 6333;
			else if (Age == 4) return 6334;
			else if (Age == 5) return 6335;
			else if (Age == 6) return 6336;
			else return 6337;
		}
		BlockState Carrots();
		unsigned char Age(BlockState Block);
	}
	namespace CartographyTable
	{
		constexpr BlockState CartographyTable()
		{
			return 14819;
		}
	}
	namespace CarvedPumpkin
	{
		constexpr BlockState CarvedPumpkin(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 4016;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 4017;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 4018;
			else return 4019;
		}
		BlockState CarvedPumpkin();
		eBlockFace Facing(BlockState Block);
	}
	namespace Cauldron
	{
		constexpr BlockState Cauldron(const unsigned char Level)
		{
			if (Level == 0) return 5141;
			else if (Level == 1) return 5142;
			else if (Level == 2) return 5143;
			else return 5144;
		}
		BlockState Cauldron();
		unsigned char Level(BlockState Block);
	}
	namespace CaveAir
	{
		constexpr BlockState CaveAir()
		{
			return 9666;
		}
	}
	namespace Chain
	{
		constexpr BlockState Chain()
		{
			return 4730;
		}
	}
	namespace ChainCommandBlock
	{
		constexpr BlockState ChainCommandBlock(const bool Conditional, const eBlockFace Facing)
		{
			if (Conditional)
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9237;
				else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9238;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9239;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9240;
				else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9241;
				else return 9242;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9243;
				else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9244;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9245;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9246;
				else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9247;
				else return 9248;
		}
		BlockState ChainCommandBlock();
		bool Conditional(BlockState Block);
		eBlockFace Facing(BlockState Block);
	}
	namespace Chest
	{
		enum class Type
		{
			Single,
			Left,
			Right
		};
		constexpr BlockState Chest(const eBlockFace Facing, const enum Type Type)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Type == Type::Single) return 2035;
				else if (Type == Type::Left) return 2037;
				else return 2039;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Type == Type::Single) return 2041;
				else if (Type == Type::Left) return 2043;
				else return 2045;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Type == Type::Single) return 2047;
				else if (Type == Type::Left) return 2049;
				else return 2051;
			else
				if (Type == Type::Single) return 2053;
				else if (Type == Type::Left) return 2055;
				else return 2057;
		}
		BlockState Chest();
		eBlockFace Facing(BlockState Block);
		enum Type Type(BlockState Block);
	}
	namespace ChippedAnvil
	{
		constexpr BlockState ChippedAnvil(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6614;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6615;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6616;
			else return 6617;
		}
		BlockState ChippedAnvil();
		eBlockFace Facing(BlockState Block);
	}
	namespace ChiseledNetherBricks
	{
		constexpr BlockState ChiseledNetherBricks()
		{
			return 17101;
		}
	}
	namespace ChiseledPolishedBlackstone
	{
		constexpr BlockState ChiseledPolishedBlackstone()
		{
			return 16253;
		}
	}
	namespace ChiseledQuartzBlock
	{
		constexpr BlockState ChiseledQuartzBlock()
		{
			return 6739;
		}
	}
	namespace ChiseledRedSandstone
	{
		constexpr BlockState ChiseledRedSandstone()
		{
			return 8218;
		}
	}
	namespace ChiseledSandstone
	{
		constexpr BlockState ChiseledSandstone()
		{
			return 247;
		}
	}
	namespace ChiseledStoneBricks
	{
		constexpr BlockState ChiseledStoneBricks()
		{
			return 4498;
		}
	}
	namespace ChorusFlower
	{
		constexpr BlockState ChorusFlower(const unsigned char Age)
		{
			if (Age == 0) return 9128;
			else if (Age == 1) return 9129;
			else if (Age == 2) return 9130;
			else if (Age == 3) return 9131;
			else if (Age == 4) return 9132;
			else return 9133;
		}
		BlockState ChorusFlower();
		unsigned char Age(BlockState Block);
	}
	namespace ChorusPlant
	{
		constexpr BlockState ChorusPlant(const bool Down, const bool East, const bool North, const bool South, const bool Up, const bool West)
		{
			if (Down)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 9064;
								else return 9065;
							else
								if (West) return 9066;
								else return 9067;
						else
							if (Up)
								if (West) return 9068;
								else return 9069;
							else
								if (West) return 9070;
								else return 9071;
					else
						if (South)
							if (Up)
								if (West) return 9072;
								else return 9073;
							else
								if (West) return 9074;
								else return 9075;
						else
							if (Up)
								if (West) return 9076;
								else return 9077;
							else
								if (West) return 9078;
								else return 9079;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 9080;
								else return 9081;
							else
								if (West) return 9082;
								else return 9083;
						else
							if (Up)
								if (West) return 9084;
								else return 9085;
							else
								if (West) return 9086;
								else return 9087;
					else
						if (South)
							if (Up)
								if (West) return 9088;
								else return 9089;
							else
								if (West) return 9090;
								else return 9091;
						else
							if (Up)
								if (West) return 9092;
								else return 9093;
							else
								if (West) return 9094;
								else return 9095;
			else
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 9096;
								else return 9097;
							else
								if (West) return 9098;
								else return 9099;
						else
							if (Up)
								if (West) return 9100;
								else return 9101;
							else
								if (West) return 9102;
								else return 9103;
					else
						if (South)
							if (Up)
								if (West) return 9104;
								else return 9105;
							else
								if (West) return 9106;
								else return 9107;
						else
							if (Up)
								if (West) return 9108;
								else return 9109;
							else
								if (West) return 9110;
								else return 9111;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 9112;
								else return 9113;
							else
								if (West) return 9114;
								else return 9115;
						else
							if (Up)
								if (West) return 9116;
								else return 9117;
							else
								if (West) return 9118;
								else return 9119;
					else
						if (South)
							if (Up)
								if (West) return 9120;
								else return 9121;
							else
								if (West) return 9122;
								else return 9123;
						else
							if (Up)
								if (West) return 9124;
								else return 9125;
							else
								if (West) return 9126;
								else return 9127;
		}
		BlockState ChorusPlant();
		bool Down(BlockState Block);
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool Up(BlockState Block);
		bool West(BlockState Block);
	}
	namespace Clay
	{
		constexpr BlockState Clay()
		{
			return 3947;
		}
	}
	namespace CoalBlock
	{
		constexpr BlockState CoalBlock()
		{
			return 7883;
		}
	}
	namespace CoalOre
	{
		constexpr BlockState CoalOre()
		{
			return 71;
		}
	}
	namespace CoarseDirt
	{
		constexpr BlockState CoarseDirt()
		{
			return 11;
		}
	}
	namespace Cobblestone
	{
		constexpr BlockState Cobblestone()
		{
			return 14;
		}
	}
	namespace CobblestoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState CobblestoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8367;
			else if (Type == Type::Bottom) return 8369;
			else return 8371;
		}
		BlockState CobblestoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace CobblestoneStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState CobblestoneStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 3656;
					else if (Shape == Shape::InnerLeft) return 3658;
					else if (Shape == Shape::InnerRight) return 3660;
					else if (Shape == Shape::OuterLeft) return 3662;
					else return 3664;
				else
					if (Shape == Shape::Straight) return 3666;
					else if (Shape == Shape::InnerLeft) return 3668;
					else if (Shape == Shape::InnerRight) return 3670;
					else if (Shape == Shape::OuterLeft) return 3672;
					else return 3674;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 3676;
					else if (Shape == Shape::InnerLeft) return 3678;
					else if (Shape == Shape::InnerRight) return 3680;
					else if (Shape == Shape::OuterLeft) return 3682;
					else return 3684;
				else
					if (Shape == Shape::Straight) return 3686;
					else if (Shape == Shape::InnerLeft) return 3688;
					else if (Shape == Shape::InnerRight) return 3690;
					else if (Shape == Shape::OuterLeft) return 3692;
					else return 3694;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 3696;
					else if (Shape == Shape::InnerLeft) return 3698;
					else if (Shape == Shape::InnerRight) return 3700;
					else if (Shape == Shape::OuterLeft) return 3702;
					else return 3704;
				else
					if (Shape == Shape::Straight) return 3706;
					else if (Shape == Shape::InnerLeft) return 3708;
					else if (Shape == Shape::InnerRight) return 3710;
					else if (Shape == Shape::OuterLeft) return 3712;
					else return 3714;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 3716;
					else if (Shape == Shape::InnerLeft) return 3718;
					else if (Shape == Shape::InnerRight) return 3720;
					else if (Shape == Shape::OuterLeft) return 3722;
					else return 3724;
				else
					if (Shape == Shape::Straight) return 3726;
					else if (Shape == Shape::InnerLeft) return 3728;
					else if (Shape == Shape::InnerRight) return 3730;
					else if (Shape == Shape::OuterLeft) return 3732;
					else return 3734;
		}
		BlockState CobblestoneStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace CobblestoneWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState CobblestoneWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 5660;
							else if (West == West::Low) return 5661;
							else return 5662;
						else
							if (West == West::None) return 5666;
							else if (West == West::Low) return 5667;
							else return 5668;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 5672;
							else if (West == West::Low) return 5673;
							else return 5674;
						else
							if (West == West::None) return 5678;
							else if (West == West::Low) return 5679;
							else return 5680;
					else
						if (Up)
							if (West == West::None) return 5684;
							else if (West == West::Low) return 5685;
							else return 5686;
						else
							if (West == West::None) return 5690;
							else if (West == West::Low) return 5691;
							else return 5692;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 5696;
							else if (West == West::Low) return 5697;
							else return 5698;
						else
							if (West == West::None) return 5702;
							else if (West == West::Low) return 5703;
							else return 5704;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 5708;
							else if (West == West::Low) return 5709;
							else return 5710;
						else
							if (West == West::None) return 5714;
							else if (West == West::Low) return 5715;
							else return 5716;
					else
						if (Up)
							if (West == West::None) return 5720;
							else if (West == West::Low) return 5721;
							else return 5722;
						else
							if (West == West::None) return 5726;
							else if (West == West::Low) return 5727;
							else return 5728;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 5732;
							else if (West == West::Low) return 5733;
							else return 5734;
						else
							if (West == West::None) return 5738;
							else if (West == West::Low) return 5739;
							else return 5740;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 5744;
							else if (West == West::Low) return 5745;
							else return 5746;
						else
							if (West == West::None) return 5750;
							else if (West == West::Low) return 5751;
							else return 5752;
					else
						if (Up)
							if (West == West::None) return 5756;
							else if (West == West::Low) return 5757;
							else return 5758;
						else
							if (West == West::None) return 5762;
							else if (West == West::Low) return 5763;
							else return 5764;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 5768;
							else if (West == West::Low) return 5769;
							else return 5770;
						else
							if (West == West::None) return 5774;
							else if (West == West::Low) return 5775;
							else return 5776;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 5780;
							else if (West == West::Low) return 5781;
							else return 5782;
						else
							if (West == West::None) return 5786;
							else if (West == West::Low) return 5787;
							else return 5788;
					else
						if (Up)
							if (West == West::None) return 5792;
							else if (West == West::Low) return 5793;
							else return 5794;
						else
							if (West == West::None) return 5798;
							else if (West == West::Low) return 5799;
							else return 5800;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 5804;
							else if (West == West::Low) return 5805;
							else return 5806;
						else
							if (West == West::None) return 5810;
							else if (West == West::Low) return 5811;
							else return 5812;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 5816;
							else if (West == West::Low) return 5817;
							else return 5818;
						else
							if (West == West::None) return 5822;
							else if (West == West::Low) return 5823;
							else return 5824;
					else
						if (Up)
							if (West == West::None) return 5828;
							else if (West == West::Low) return 5829;
							else return 5830;
						else
							if (West == West::None) return 5834;
							else if (West == West::Low) return 5835;
							else return 5836;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 5840;
							else if (West == West::Low) return 5841;
							else return 5842;
						else
							if (West == West::None) return 5846;
							else if (West == West::Low) return 5847;
							else return 5848;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 5852;
							else if (West == West::Low) return 5853;
							else return 5854;
						else
							if (West == West::None) return 5858;
							else if (West == West::Low) return 5859;
							else return 5860;
					else
						if (Up)
							if (West == West::None) return 5864;
							else if (West == West::Low) return 5865;
							else return 5866;
						else
							if (West == West::None) return 5870;
							else if (West == West::Low) return 5871;
							else return 5872;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 5876;
							else if (West == West::Low) return 5877;
							else return 5878;
						else
							if (West == West::None) return 5882;
							else if (West == West::Low) return 5883;
							else return 5884;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 5888;
							else if (West == West::Low) return 5889;
							else return 5890;
						else
							if (West == West::None) return 5894;
							else if (West == West::Low) return 5895;
							else return 5896;
					else
						if (Up)
							if (West == West::None) return 5900;
							else if (West == West::Low) return 5901;
							else return 5902;
						else
							if (West == West::None) return 5906;
							else if (West == West::Low) return 5907;
							else return 5908;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 5912;
							else if (West == West::Low) return 5913;
							else return 5914;
						else
							if (West == West::None) return 5918;
							else if (West == West::Low) return 5919;
							else return 5920;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 5924;
							else if (West == West::Low) return 5925;
							else return 5926;
						else
							if (West == West::None) return 5930;
							else if (West == West::Low) return 5931;
							else return 5932;
					else
						if (Up)
							if (West == West::None) return 5936;
							else if (West == West::Low) return 5937;
							else return 5938;
						else
							if (West == West::None) return 5942;
							else if (West == West::Low) return 5943;
							else return 5944;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 5948;
							else if (West == West::Low) return 5949;
							else return 5950;
						else
							if (West == West::None) return 5954;
							else if (West == West::Low) return 5955;
							else return 5956;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 5960;
							else if (West == West::Low) return 5961;
							else return 5962;
						else
							if (West == West::None) return 5966;
							else if (West == West::Low) return 5967;
							else return 5968;
					else
						if (Up)
							if (West == West::None) return 5972;
							else if (West == West::Low) return 5973;
							else return 5974;
						else
							if (West == West::None) return 5978;
							else if (West == West::Low) return 5979;
							else return 5980;
		}
		BlockState CobblestoneWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace Cobweb
	{
		constexpr BlockState Cobweb()
		{
			return 1341;
		}
	}
	namespace Cocoa
	{
		constexpr BlockState Cocoa(const unsigned char Age, const eBlockFace Facing)
		{
			if (Age == 0)
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 5158;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 5159;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 5160;
				else return 5161;
			else if (Age == 1)
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 5162;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 5163;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 5164;
				else return 5165;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 5166;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 5167;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 5168;
				else return 5169;
		}
		BlockState Cocoa();
		unsigned char Age(BlockState Block);
		eBlockFace Facing(BlockState Block);
	}
	namespace CommandBlock
	{
		constexpr BlockState CommandBlock(const bool Conditional, const eBlockFace Facing)
		{
			if (Conditional)
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 5644;
				else if (Facing == eBlockFace::BLOCK_FACE_XP) return 5645;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 5646;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 5647;
				else if (Facing == eBlockFace::BLOCK_FACE_YP) return 5648;
				else return 5649;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 5650;
				else if (Facing == eBlockFace::BLOCK_FACE_XP) return 5651;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 5652;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 5653;
				else if (Facing == eBlockFace::BLOCK_FACE_YP) return 5654;
				else return 5655;
		}
		BlockState CommandBlock();
		bool Conditional(BlockState Block);
		eBlockFace Facing(BlockState Block);
	}
	namespace Comparator
	{
		enum class Mode
		{
			Compare,
			Subtract
		};
		constexpr BlockState Comparator(const eBlockFace Facing, const enum Mode Mode, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Mode == Mode::Compare)
					if (Powered) return 6678;
					else return 6679;
				else
					if (Powered) return 6680;
					else return 6681;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Mode == Mode::Compare)
					if (Powered) return 6682;
					else return 6683;
				else
					if (Powered) return 6684;
					else return 6685;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Mode == Mode::Compare)
					if (Powered) return 6686;
					else return 6687;
				else
					if (Powered) return 6688;
					else return 6689;
			else
				if (Mode == Mode::Compare)
					if (Powered) return 6690;
					else return 6691;
				else
					if (Powered) return 6692;
					else return 6693;
		}
		BlockState Comparator();
		eBlockFace Facing(BlockState Block);
		enum Mode Mode(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace Composter
	{
		constexpr BlockState Composter(const unsigned char Level)
		{
			if (Level == 0) return 15751;
			else if (Level == 1) return 15752;
			else if (Level == 2) return 15753;
			else if (Level == 3) return 15754;
			else if (Level == 4) return 15755;
			else if (Level == 5) return 15756;
			else if (Level == 6) return 15757;
			else if (Level == 7) return 15758;
			else return 15759;
		}
		BlockState Composter();
		unsigned char Level(BlockState Block);
	}
	namespace Conduit
	{
		constexpr BlockState Conduit()
		{
			return 9650;
		}
	}
	namespace Cornflower
	{
		constexpr BlockState Cornflower()
		{
			return 1422;
		}
	}
	namespace CrackedNetherBricks
	{
		constexpr BlockState CrackedNetherBricks()
		{
			return 17102;
		}
	}
	namespace CrackedPolishedBlackstoneBricks
	{
		constexpr BlockState CrackedPolishedBlackstoneBricks()
		{
			return 16252;
		}
	}
	namespace CrackedStoneBricks
	{
		constexpr BlockState CrackedStoneBricks()
		{
			return 4497;
		}
	}
	namespace CraftingTable
	{
		constexpr BlockState CraftingTable()
		{
			return 3356;
		}
	}
	namespace CreeperHead
	{
		constexpr BlockState CreeperHead(const unsigned char Rotation)
		{
			if (Rotation == 0) return 6570;
			else if (Rotation == 1) return 6571;
			else if (Rotation == 2) return 6572;
			else if (Rotation == 3) return 6573;
			else if (Rotation == 4) return 6574;
			else if (Rotation == 5) return 6575;
			else if (Rotation == 6) return 6576;
			else if (Rotation == 7) return 6577;
			else if (Rotation == 8) return 6578;
			else if (Rotation == 9) return 6579;
			else if (Rotation == 10) return 6580;
			else if (Rotation == 11) return 6581;
			else if (Rotation == 12) return 6582;
			else if (Rotation == 13) return 6583;
			else if (Rotation == 14) return 6584;
			else return 6585;
		}
		BlockState CreeperHead();
		unsigned char Rotation(BlockState Block);
	}
	namespace CreeperWallHead
	{
		constexpr BlockState CreeperWallHead(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6586;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6587;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6588;
			else return 6589;
		}
		BlockState CreeperWallHead();
		eBlockFace Facing(BlockState Block);
	}
	namespace CrimsonButton
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState CrimsonButton(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 15479;
					else return 15480;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 15481;
					else return 15482;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 15483;
					else return 15484;
				else
					if (Powered) return 15485;
					else return 15486;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 15487;
					else return 15488;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 15489;
					else return 15490;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 15491;
					else return 15492;
				else
					if (Powered) return 15493;
					else return 15494;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 15495;
					else return 15496;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 15497;
					else return 15498;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 15499;
					else return 15500;
				else
					if (Powered) return 15501;
					else return 15502;
		}
		BlockState CrimsonButton();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace CrimsonDoor
	{
		enum class Half
		{
			Upper,
			Lower
		};
		enum class Hinge
		{
			Left,
			Right
		};
		constexpr BlockState CrimsonDoor(const eBlockFace Facing, const enum Half Half, const enum Hinge Hinge, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15527;
							else return 15528;
						else
							if (Powered) return 15529;
							else return 15530;
					else
						if (Open)
							if (Powered) return 15531;
							else return 15532;
						else
							if (Powered) return 15533;
							else return 15534;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15535;
							else return 15536;
						else
							if (Powered) return 15537;
							else return 15538;
					else
						if (Open)
							if (Powered) return 15539;
							else return 15540;
						else
							if (Powered) return 15541;
							else return 15542;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15543;
							else return 15544;
						else
							if (Powered) return 15545;
							else return 15546;
					else
						if (Open)
							if (Powered) return 15547;
							else return 15548;
						else
							if (Powered) return 15549;
							else return 15550;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15551;
							else return 15552;
						else
							if (Powered) return 15553;
							else return 15554;
					else
						if (Open)
							if (Powered) return 15555;
							else return 15556;
						else
							if (Powered) return 15557;
							else return 15558;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15559;
							else return 15560;
						else
							if (Powered) return 15561;
							else return 15562;
					else
						if (Open)
							if (Powered) return 15563;
							else return 15564;
						else
							if (Powered) return 15565;
							else return 15566;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15567;
							else return 15568;
						else
							if (Powered) return 15569;
							else return 15570;
					else
						if (Open)
							if (Powered) return 15571;
							else return 15572;
						else
							if (Powered) return 15573;
							else return 15574;
			else
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15575;
							else return 15576;
						else
							if (Powered) return 15577;
							else return 15578;
					else
						if (Open)
							if (Powered) return 15579;
							else return 15580;
						else
							if (Powered) return 15581;
							else return 15582;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15583;
							else return 15584;
						else
							if (Powered) return 15585;
							else return 15586;
					else
						if (Open)
							if (Powered) return 15587;
							else return 15588;
						else
							if (Powered) return 15589;
							else return 15590;
		}
		BlockState CrimsonDoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Hinge Hinge(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace CrimsonFence
	{
		constexpr BlockState CrimsonFence(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 15065;
						else return 15066;
					else
						if (West) return 15069;
						else return 15070;
				else
					if (South)
						if (West) return 15073;
						else return 15074;
					else
						if (West) return 15077;
						else return 15078;
			else
				if (North)
					if (South)
						if (West) return 15081;
						else return 15082;
					else
						if (West) return 15085;
						else return 15086;
				else
					if (South)
						if (West) return 15089;
						else return 15090;
					else
						if (West) return 15093;
						else return 15094;
		}
		BlockState CrimsonFence();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace CrimsonFenceGate
	{
		constexpr BlockState CrimsonFenceGate(const eBlockFace Facing, const bool InWall, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (InWall)
					if (Open)
						if (Powered) return 15255;
						else return 15256;
					else
						if (Powered) return 15257;
						else return 15258;
				else
					if (Open)
						if (Powered) return 15259;
						else return 15260;
					else
						if (Powered) return 15261;
						else return 15262;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (InWall)
					if (Open)
						if (Powered) return 15263;
						else return 15264;
					else
						if (Powered) return 15265;
						else return 15266;
				else
					if (Open)
						if (Powered) return 15267;
						else return 15268;
					else
						if (Powered) return 15269;
						else return 15270;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (InWall)
					if (Open)
						if (Powered) return 15271;
						else return 15272;
					else
						if (Powered) return 15273;
						else return 15274;
				else
					if (Open)
						if (Powered) return 15275;
						else return 15276;
					else
						if (Powered) return 15277;
						else return 15278;
			else
				if (InWall)
					if (Open)
						if (Powered) return 15279;
						else return 15280;
					else
						if (Powered) return 15281;
						else return 15282;
				else
					if (Open)
						if (Powered) return 15283;
						else return 15284;
					else
						if (Powered) return 15285;
						else return 15286;
		}
		BlockState CrimsonFenceGate();
		eBlockFace Facing(BlockState Block);
		bool InWall(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace CrimsonFungus
	{
		constexpr BlockState CrimsonFungus()
		{
			return 14988;
		}
	}
	namespace CrimsonHyphae
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState CrimsonHyphae(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 14981;
			else if (Axis == Axis::Y) return 14982;
			else return 14983;
		}
		BlockState CrimsonHyphae();
		enum Axis Axis(BlockState Block);
	}
	namespace CrimsonNylium
	{
		constexpr BlockState CrimsonNylium()
		{
			return 14987;
		}
	}
	namespace CrimsonPlanks
	{
		constexpr BlockState CrimsonPlanks()
		{
			return 15045;
		}
	}
	namespace CrimsonPressurePlate
	{
		constexpr BlockState CrimsonPressurePlate(const bool Powered)
		{
			if (Powered) return 15059;
			else return 15060;
		}
		BlockState CrimsonPressurePlate();
		bool Powered(BlockState Block);
	}
	namespace CrimsonRoots
	{
		constexpr BlockState CrimsonRoots()
		{
			return 15044;
		}
	}
	namespace CrimsonSign
	{
		constexpr BlockState CrimsonSign(const unsigned char Rotation)
		{
			if (Rotation == 0) return 15656;
			else if (Rotation == 1) return 15658;
			else if (Rotation == 2) return 15660;
			else if (Rotation == 3) return 15662;
			else if (Rotation == 4) return 15664;
			else if (Rotation == 5) return 15666;
			else if (Rotation == 6) return 15668;
			else if (Rotation == 7) return 15670;
			else if (Rotation == 8) return 15672;
			else if (Rotation == 9) return 15674;
			else if (Rotation == 10) return 15676;
			else if (Rotation == 11) return 15678;
			else if (Rotation == 12) return 15680;
			else if (Rotation == 13) return 15682;
			else if (Rotation == 14) return 15684;
			else return 15686;
		}
		BlockState CrimsonSign();
		unsigned char Rotation(BlockState Block);
	}
	namespace CrimsonSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState CrimsonSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 15048;
			else if (Type == Type::Bottom) return 15050;
			else return 15052;
		}
		BlockState CrimsonSlab();
		enum Type Type(BlockState Block);
	}
	namespace CrimsonStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState CrimsonStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15320;
					else if (Shape == Shape::InnerLeft) return 15322;
					else if (Shape == Shape::InnerRight) return 15324;
					else if (Shape == Shape::OuterLeft) return 15326;
					else return 15328;
				else
					if (Shape == Shape::Straight) return 15330;
					else if (Shape == Shape::InnerLeft) return 15332;
					else if (Shape == Shape::InnerRight) return 15334;
					else if (Shape == Shape::OuterLeft) return 15336;
					else return 15338;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15340;
					else if (Shape == Shape::InnerLeft) return 15342;
					else if (Shape == Shape::InnerRight) return 15344;
					else if (Shape == Shape::OuterLeft) return 15346;
					else return 15348;
				else
					if (Shape == Shape::Straight) return 15350;
					else if (Shape == Shape::InnerLeft) return 15352;
					else if (Shape == Shape::InnerRight) return 15354;
					else if (Shape == Shape::OuterLeft) return 15356;
					else return 15358;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15360;
					else if (Shape == Shape::InnerLeft) return 15362;
					else if (Shape == Shape::InnerRight) return 15364;
					else if (Shape == Shape::OuterLeft) return 15366;
					else return 15368;
				else
					if (Shape == Shape::Straight) return 15370;
					else if (Shape == Shape::InnerLeft) return 15372;
					else if (Shape == Shape::InnerRight) return 15374;
					else if (Shape == Shape::OuterLeft) return 15376;
					else return 15378;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15380;
					else if (Shape == Shape::InnerLeft) return 15382;
					else if (Shape == Shape::InnerRight) return 15384;
					else if (Shape == Shape::OuterLeft) return 15386;
					else return 15388;
				else
					if (Shape == Shape::Straight) return 15390;
					else if (Shape == Shape::InnerLeft) return 15392;
					else if (Shape == Shape::InnerRight) return 15394;
					else if (Shape == Shape::OuterLeft) return 15396;
					else return 15398;
		}
		BlockState CrimsonStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace CrimsonStem
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState CrimsonStem(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 14975;
			else if (Axis == Axis::Y) return 14976;
			else return 14977;
		}
		BlockState CrimsonStem();
		enum Axis Axis(BlockState Block);
	}
	namespace CrimsonTrapdoor
	{
		enum class Half
		{
			Top,
			Bottom
		};
		constexpr BlockState CrimsonTrapdoor(const eBlockFace Facing, const enum Half Half, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 15128;
						else return 15130;
					else
						if (Powered) return 15132;
						else return 15134;
				else
					if (Open)
						if (Powered) return 15136;
						else return 15138;
					else
						if (Powered) return 15140;
						else return 15142;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 15144;
						else return 15146;
					else
						if (Powered) return 15148;
						else return 15150;
				else
					if (Open)
						if (Powered) return 15152;
						else return 15154;
					else
						if (Powered) return 15156;
						else return 15158;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 15160;
						else return 15162;
					else
						if (Powered) return 15164;
						else return 15166;
				else
					if (Open)
						if (Powered) return 15168;
						else return 15170;
					else
						if (Powered) return 15172;
						else return 15174;
			else
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 15176;
						else return 15178;
					else
						if (Powered) return 15180;
						else return 15182;
				else
					if (Open)
						if (Powered) return 15184;
						else return 15186;
					else
						if (Powered) return 15188;
						else return 15190;
		}
		BlockState CrimsonTrapdoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace CrimsonWallSign
	{
		constexpr BlockState CrimsonWallSign(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 15720;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 15722;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 15724;
			else return 15726;
		}
		BlockState CrimsonWallSign();
		eBlockFace Facing(BlockState Block);
	}
	namespace CryingObsidian
	{
		constexpr BlockState CryingObsidian()
		{
			return 15828;
		}
	}
	namespace CutRedSandstone
	{
		constexpr BlockState CutRedSandstone()
		{
			return 8219;
		}
	}
	namespace CutRedSandstoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState CutRedSandstoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8403;
			else if (Type == Type::Bottom) return 8405;
			else return 8407;
		}
		BlockState CutRedSandstoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace CutSandstone
	{
		constexpr BlockState CutSandstone()
		{
			return 248;
		}
	}
	namespace CutSandstoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState CutSandstoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8355;
			else if (Type == Type::Bottom) return 8357;
			else return 8359;
		}
		BlockState CutSandstoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace CyanBanner
	{
		constexpr BlockState CyanBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 8041;
			else if (Rotation == 1) return 8042;
			else if (Rotation == 2) return 8043;
			else if (Rotation == 3) return 8044;
			else if (Rotation == 4) return 8045;
			else if (Rotation == 5) return 8046;
			else if (Rotation == 6) return 8047;
			else if (Rotation == 7) return 8048;
			else if (Rotation == 8) return 8049;
			else if (Rotation == 9) return 8050;
			else if (Rotation == 10) return 8051;
			else if (Rotation == 11) return 8052;
			else if (Rotation == 12) return 8053;
			else if (Rotation == 13) return 8054;
			else if (Rotation == 14) return 8055;
			else return 8056;
		}
		BlockState CyanBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace CyanBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState CyanBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1193;
					else return 1194;
				else
					if (Part == Part::Head) return 1195;
					else return 1196;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1197;
					else return 1198;
				else
					if (Part == Part::Head) return 1199;
					else return 1200;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1201;
					else return 1202;
				else
					if (Part == Part::Head) return 1203;
					else return 1204;
			else
				if (Occupied)
					if (Part == Part::Head) return 1205;
					else return 1206;
				else
					if (Part == Part::Head) return 1207;
					else return 1208;
		}
		BlockState CyanBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace CyanCarpet
	{
		constexpr BlockState CyanCarpet()
		{
			return 7875;
		}
	}
	namespace CyanConcrete
	{
		constexpr BlockState CyanConcrete()
		{
			return 9447;
		}
	}
	namespace CyanConcretePowder
	{
		constexpr BlockState CyanConcretePowder()
		{
			return 9463;
		}
	}
	namespace CyanGlazedTerracotta
	{
		constexpr BlockState CyanGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9410;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9411;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9412;
			else return 9413;
		}
		BlockState CyanGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace CyanShulkerBox
	{
		constexpr BlockState CyanShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9332;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9333;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9334;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9335;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9336;
			else return 9337;
		}
		BlockState CyanShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace CyanStainedGlass
	{
		constexpr BlockState CyanStainedGlass()
		{
			return 4104;
		}
	}
	namespace CyanStainedGlassPane
	{
		constexpr BlockState CyanStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7153;
						else return 7154;
					else
						if (West) return 7157;
						else return 7158;
				else
					if (South)
						if (West) return 7161;
						else return 7162;
					else
						if (West) return 7165;
						else return 7166;
			else
				if (North)
					if (South)
						if (West) return 7169;
						else return 7170;
					else
						if (West) return 7173;
						else return 7174;
				else
					if (South)
						if (West) return 7177;
						else return 7178;
					else
						if (West) return 7181;
						else return 7182;
		}
		BlockState CyanStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace CyanTerracotta
	{
		constexpr BlockState CyanTerracotta()
		{
			return 6856;
		}
	}
	namespace CyanWallBanner
	{
		constexpr BlockState CyanWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8189;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8190;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8191;
			else return 8192;
		}
		BlockState CyanWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace CyanWool
	{
		constexpr BlockState CyanWool()
		{
			return 1393;
		}
	}
	namespace DamagedAnvil
	{
		constexpr BlockState DamagedAnvil(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6618;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6619;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6620;
			else return 6621;
		}
		BlockState DamagedAnvil();
		eBlockFace Facing(BlockState Block);
	}
	namespace Dandelion
	{
		constexpr BlockState Dandelion()
		{
			return 1412;
		}
	}
	namespace DarkOakButton
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState DarkOakButton(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6466;
					else return 6467;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6468;
					else return 6469;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6470;
					else return 6471;
				else
					if (Powered) return 6472;
					else return 6473;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6474;
					else return 6475;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6476;
					else return 6477;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6478;
					else return 6479;
				else
					if (Powered) return 6480;
					else return 6481;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6482;
					else return 6483;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6484;
					else return 6485;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6486;
					else return 6487;
				else
					if (Powered) return 6488;
					else return 6489;
		}
		BlockState DarkOakButton();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace DarkOakDoor
	{
		enum class Half
		{
			Upper,
			Lower
		};
		enum class Hinge
		{
			Left,
			Right
		};
		constexpr BlockState DarkOakDoor(const eBlockFace Facing, const enum Half Half, const enum Hinge Hinge, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8994;
							else return 8995;
						else
							if (Powered) return 8996;
							else return 8997;
					else
						if (Open)
							if (Powered) return 8998;
							else return 8999;
						else
							if (Powered) return 9000;
							else return 9001;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 9002;
							else return 9003;
						else
							if (Powered) return 9004;
							else return 9005;
					else
						if (Open)
							if (Powered) return 9006;
							else return 9007;
						else
							if (Powered) return 9008;
							else return 9009;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 9010;
							else return 9011;
						else
							if (Powered) return 9012;
							else return 9013;
					else
						if (Open)
							if (Powered) return 9014;
							else return 9015;
						else
							if (Powered) return 9016;
							else return 9017;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 9018;
							else return 9019;
						else
							if (Powered) return 9020;
							else return 9021;
					else
						if (Open)
							if (Powered) return 9022;
							else return 9023;
						else
							if (Powered) return 9024;
							else return 9025;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 9026;
							else return 9027;
						else
							if (Powered) return 9028;
							else return 9029;
					else
						if (Open)
							if (Powered) return 9030;
							else return 9031;
						else
							if (Powered) return 9032;
							else return 9033;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 9034;
							else return 9035;
						else
							if (Powered) return 9036;
							else return 9037;
					else
						if (Open)
							if (Powered) return 9038;
							else return 9039;
						else
							if (Powered) return 9040;
							else return 9041;
			else
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 9042;
							else return 9043;
						else
							if (Powered) return 9044;
							else return 9045;
					else
						if (Open)
							if (Powered) return 9046;
							else return 9047;
						else
							if (Powered) return 9048;
							else return 9049;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 9050;
							else return 9051;
						else
							if (Powered) return 9052;
							else return 9053;
					else
						if (Open)
							if (Powered) return 9054;
							else return 9055;
						else
							if (Powered) return 9056;
							else return 9057;
		}
		BlockState DarkOakDoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Hinge Hinge(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace DarkOakFence
	{
		constexpr BlockState DarkOakFence(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 8708;
						else return 8709;
					else
						if (West) return 8712;
						else return 8713;
				else
					if (South)
						if (West) return 8716;
						else return 8717;
					else
						if (West) return 8720;
						else return 8721;
			else
				if (North)
					if (South)
						if (West) return 8724;
						else return 8725;
					else
						if (West) return 8728;
						else return 8729;
				else
					if (South)
						if (West) return 8732;
						else return 8733;
					else
						if (West) return 8736;
						else return 8737;
		}
		BlockState DarkOakFence();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace DarkOakFenceGate
	{
		constexpr BlockState DarkOakFenceGate(const eBlockFace Facing, const bool InWall, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (InWall)
					if (Open)
						if (Powered) return 8546;
						else return 8547;
					else
						if (Powered) return 8548;
						else return 8549;
				else
					if (Open)
						if (Powered) return 8550;
						else return 8551;
					else
						if (Powered) return 8552;
						else return 8553;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (InWall)
					if (Open)
						if (Powered) return 8554;
						else return 8555;
					else
						if (Powered) return 8556;
						else return 8557;
				else
					if (Open)
						if (Powered) return 8558;
						else return 8559;
					else
						if (Powered) return 8560;
						else return 8561;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (InWall)
					if (Open)
						if (Powered) return 8562;
						else return 8563;
					else
						if (Powered) return 8564;
						else return 8565;
				else
					if (Open)
						if (Powered) return 8566;
						else return 8567;
					else
						if (Powered) return 8568;
						else return 8569;
			else
				if (InWall)
					if (Open)
						if (Powered) return 8570;
						else return 8571;
					else
						if (Powered) return 8572;
						else return 8573;
				else
					if (Open)
						if (Powered) return 8574;
						else return 8575;
					else
						if (Powered) return 8576;
						else return 8577;
		}
		BlockState DarkOakFenceGate();
		eBlockFace Facing(BlockState Block);
		bool InWall(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace DarkOakLeaves
	{
		constexpr BlockState DarkOakLeaves(const unsigned char Distance, const bool Persistent)
		{
			if (Distance == 1)
				if (Persistent) return 215;
				else return 216;
			else if (Distance == 2)
				if (Persistent) return 217;
				else return 218;
			else if (Distance == 3)
				if (Persistent) return 219;
				else return 220;
			else if (Distance == 4)
				if (Persistent) return 221;
				else return 222;
			else if (Distance == 5)
				if (Persistent) return 223;
				else return 224;
			else if (Distance == 6)
				if (Persistent) return 225;
				else return 226;
			else
				if (Persistent) return 227;
				else return 228;
		}
		BlockState DarkOakLeaves();
		unsigned char Distance(BlockState Block);
		bool Persistent(BlockState Block);
	}
	namespace DarkOakLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState DarkOakLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 88;
			else if (Axis == Axis::Y) return 89;
			else return 90;
		}
		BlockState DarkOakLog();
		enum Axis Axis(BlockState Block);
	}
	namespace DarkOakPlanks
	{
		constexpr BlockState DarkOakPlanks()
		{
			return 20;
		}
	}
	namespace DarkOakPressurePlate
	{
		constexpr BlockState DarkOakPressurePlate(const bool Powered)
		{
			if (Powered) return 3883;
			else return 3884;
		}
		BlockState DarkOakPressurePlate();
		bool Powered(BlockState Block);
	}
	namespace DarkOakSapling
	{
		constexpr BlockState DarkOakSapling(const unsigned char Stage)
		{
			if (Stage == 0) return 31;
			else return 32;
		}
		BlockState DarkOakSapling();
		unsigned char Stage(BlockState Block);
	}
	namespace DarkOakSign
	{
		constexpr BlockState DarkOakSign(const unsigned char Rotation)
		{
			if (Rotation == 0) return 3542;
			else if (Rotation == 1) return 3544;
			else if (Rotation == 2) return 3546;
			else if (Rotation == 3) return 3548;
			else if (Rotation == 4) return 3550;
			else if (Rotation == 5) return 3552;
			else if (Rotation == 6) return 3554;
			else if (Rotation == 7) return 3556;
			else if (Rotation == 8) return 3558;
			else if (Rotation == 9) return 3560;
			else if (Rotation == 10) return 3562;
			else if (Rotation == 11) return 3564;
			else if (Rotation == 12) return 3566;
			else if (Rotation == 13) return 3568;
			else if (Rotation == 14) return 3570;
			else return 3572;
		}
		BlockState DarkOakSign();
		unsigned char Rotation(BlockState Block);
	}
	namespace DarkOakSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState DarkOakSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8331;
			else if (Type == Type::Bottom) return 8333;
			else return 8335;
		}
		BlockState DarkOakSlab();
		enum Type Type(BlockState Block);
	}
	namespace DarkOakStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState DarkOakStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7456;
					else if (Shape == Shape::InnerLeft) return 7458;
					else if (Shape == Shape::InnerRight) return 7460;
					else if (Shape == Shape::OuterLeft) return 7462;
					else return 7464;
				else
					if (Shape == Shape::Straight) return 7466;
					else if (Shape == Shape::InnerLeft) return 7468;
					else if (Shape == Shape::InnerRight) return 7470;
					else if (Shape == Shape::OuterLeft) return 7472;
					else return 7474;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7476;
					else if (Shape == Shape::InnerLeft) return 7478;
					else if (Shape == Shape::InnerRight) return 7480;
					else if (Shape == Shape::OuterLeft) return 7482;
					else return 7484;
				else
					if (Shape == Shape::Straight) return 7486;
					else if (Shape == Shape::InnerLeft) return 7488;
					else if (Shape == Shape::InnerRight) return 7490;
					else if (Shape == Shape::OuterLeft) return 7492;
					else return 7494;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7496;
					else if (Shape == Shape::InnerLeft) return 7498;
					else if (Shape == Shape::InnerRight) return 7500;
					else if (Shape == Shape::OuterLeft) return 7502;
					else return 7504;
				else
					if (Shape == Shape::Straight) return 7506;
					else if (Shape == Shape::InnerLeft) return 7508;
					else if (Shape == Shape::InnerRight) return 7510;
					else if (Shape == Shape::OuterLeft) return 7512;
					else return 7514;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7516;
					else if (Shape == Shape::InnerLeft) return 7518;
					else if (Shape == Shape::InnerRight) return 7520;
					else if (Shape == Shape::OuterLeft) return 7522;
					else return 7524;
				else
					if (Shape == Shape::Straight) return 7526;
					else if (Shape == Shape::InnerLeft) return 7528;
					else if (Shape == Shape::InnerRight) return 7530;
					else if (Shape == Shape::OuterLeft) return 7532;
					else return 7534;
		}
		BlockState DarkOakStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace DarkOakTrapdoor
	{
		enum class Half
		{
			Top,
			Bottom
		};
		constexpr BlockState DarkOakTrapdoor(const eBlockFace Facing, const enum Half Half, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4432;
						else return 4434;
					else
						if (Powered) return 4436;
						else return 4438;
				else
					if (Open)
						if (Powered) return 4440;
						else return 4442;
					else
						if (Powered) return 4444;
						else return 4446;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4448;
						else return 4450;
					else
						if (Powered) return 4452;
						else return 4454;
				else
					if (Open)
						if (Powered) return 4456;
						else return 4458;
					else
						if (Powered) return 4460;
						else return 4462;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4464;
						else return 4466;
					else
						if (Powered) return 4468;
						else return 4470;
				else
					if (Open)
						if (Powered) return 4472;
						else return 4474;
					else
						if (Powered) return 4476;
						else return 4478;
			else
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4480;
						else return 4482;
					else
						if (Powered) return 4484;
						else return 4486;
				else
					if (Open)
						if (Powered) return 4488;
						else return 4490;
					else
						if (Powered) return 4492;
						else return 4494;
		}
		BlockState DarkOakTrapdoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace DarkOakWallSign
	{
		constexpr BlockState DarkOakWallSign(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 3776;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 3778;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 3780;
			else return 3782;
		}
		BlockState DarkOakWallSign();
		eBlockFace Facing(BlockState Block);
	}
	namespace DarkOakWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState DarkOakWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 124;
			else if (Axis == Axis::Y) return 125;
			else return 126;
		}
		BlockState DarkOakWood();
		enum Axis Axis(BlockState Block);
	}
	namespace DarkPrismarine
	{
		constexpr BlockState DarkPrismarine()
		{
			return 7603;
		}
	}
	namespace DarkPrismarineSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState DarkPrismarineSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 7857;
			else if (Type == Type::Bottom) return 7859;
			else return 7861;
		}
		BlockState DarkPrismarineSlab();
		enum Type Type(BlockState Block);
	}
	namespace DarkPrismarineStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState DarkPrismarineStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7765;
					else if (Shape == Shape::InnerLeft) return 7767;
					else if (Shape == Shape::InnerRight) return 7769;
					else if (Shape == Shape::OuterLeft) return 7771;
					else return 7773;
				else
					if (Shape == Shape::Straight) return 7775;
					else if (Shape == Shape::InnerLeft) return 7777;
					else if (Shape == Shape::InnerRight) return 7779;
					else if (Shape == Shape::OuterLeft) return 7781;
					else return 7783;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7785;
					else if (Shape == Shape::InnerLeft) return 7787;
					else if (Shape == Shape::InnerRight) return 7789;
					else if (Shape == Shape::OuterLeft) return 7791;
					else return 7793;
				else
					if (Shape == Shape::Straight) return 7795;
					else if (Shape == Shape::InnerLeft) return 7797;
					else if (Shape == Shape::InnerRight) return 7799;
					else if (Shape == Shape::OuterLeft) return 7801;
					else return 7803;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7805;
					else if (Shape == Shape::InnerLeft) return 7807;
					else if (Shape == Shape::InnerRight) return 7809;
					else if (Shape == Shape::OuterLeft) return 7811;
					else return 7813;
				else
					if (Shape == Shape::Straight) return 7815;
					else if (Shape == Shape::InnerLeft) return 7817;
					else if (Shape == Shape::InnerRight) return 7819;
					else if (Shape == Shape::OuterLeft) return 7821;
					else return 7823;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7825;
					else if (Shape == Shape::InnerLeft) return 7827;
					else if (Shape == Shape::InnerRight) return 7829;
					else if (Shape == Shape::OuterLeft) return 7831;
					else return 7833;
				else
					if (Shape == Shape::Straight) return 7835;
					else if (Shape == Shape::InnerLeft) return 7837;
					else if (Shape == Shape::InnerRight) return 7839;
					else if (Shape == Shape::OuterLeft) return 7841;
					else return 7843;
		}
		BlockState DarkPrismarineStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace DaylightDetector
	{
		constexpr BlockState DaylightDetector(const bool Inverted, const unsigned char Power)
		{
			if (Inverted)
				if (Power == 0) return 6694;
				else if (Power == 1) return 6695;
				else if (Power == 2) return 6696;
				else if (Power == 3) return 6697;
				else if (Power == 4) return 6698;
				else if (Power == 5) return 6699;
				else if (Power == 6) return 6700;
				else if (Power == 7) return 6701;
				else if (Power == 8) return 6702;
				else if (Power == 9) return 6703;
				else if (Power == 10) return 6704;
				else if (Power == 11) return 6705;
				else if (Power == 12) return 6706;
				else if (Power == 13) return 6707;
				else if (Power == 14) return 6708;
				else return 6709;
			else
				if (Power == 0) return 6710;
				else if (Power == 1) return 6711;
				else if (Power == 2) return 6712;
				else if (Power == 3) return 6713;
				else if (Power == 4) return 6714;
				else if (Power == 5) return 6715;
				else if (Power == 6) return 6716;
				else if (Power == 7) return 6717;
				else if (Power == 8) return 6718;
				else if (Power == 9) return 6719;
				else if (Power == 10) return 6720;
				else if (Power == 11) return 6721;
				else if (Power == 12) return 6722;
				else if (Power == 13) return 6723;
				else if (Power == 14) return 6724;
				else return 6725;
		}
		BlockState DaylightDetector();
		bool Inverted(BlockState Block);
		unsigned char Power(BlockState Block);
	}
	namespace DeadBrainCoral
	{
		constexpr BlockState DeadBrainCoral()
		{
			return 9523;
		}
	}
	namespace DeadBrainCoralBlock
	{
		constexpr BlockState DeadBrainCoralBlock()
		{
			return 9511;
		}
	}
	namespace DeadBrainCoralFan
	{
		constexpr BlockState DeadBrainCoralFan()
		{
			return 9543;
		}
	}
	namespace DeadBrainCoralWallFan
	{
		constexpr BlockState DeadBrainCoralWallFan(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9569;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9571;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9573;
			else return 9575;
		}
		BlockState DeadBrainCoralWallFan();
		eBlockFace Facing(BlockState Block);
	}
	namespace DeadBubbleCoral
	{
		constexpr BlockState DeadBubbleCoral()
		{
			return 9525;
		}
	}
	namespace DeadBubbleCoralBlock
	{
		constexpr BlockState DeadBubbleCoralBlock()
		{
			return 9512;
		}
	}
	namespace DeadBubbleCoralFan
	{
		constexpr BlockState DeadBubbleCoralFan()
		{
			return 9545;
		}
	}
	namespace DeadBubbleCoralWallFan
	{
		constexpr BlockState DeadBubbleCoralWallFan(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9577;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9579;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9581;
			else return 9583;
		}
		BlockState DeadBubbleCoralWallFan();
		eBlockFace Facing(BlockState Block);
	}
	namespace DeadBush
	{
		constexpr BlockState DeadBush()
		{
			return 1344;
		}
	}
	namespace DeadFireCoral
	{
		constexpr BlockState DeadFireCoral()
		{
			return 9527;
		}
	}
	namespace DeadFireCoralBlock
	{
		constexpr BlockState DeadFireCoralBlock()
		{
			return 9513;
		}
	}
	namespace DeadFireCoralFan
	{
		constexpr BlockState DeadFireCoralFan()
		{
			return 9547;
		}
	}
	namespace DeadFireCoralWallFan
	{
		constexpr BlockState DeadFireCoralWallFan(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9585;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9587;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9589;
			else return 9591;
		}
		BlockState DeadFireCoralWallFan();
		eBlockFace Facing(BlockState Block);
	}
	namespace DeadHornCoral
	{
		constexpr BlockState DeadHornCoral()
		{
			return 9529;
		}
	}
	namespace DeadHornCoralBlock
	{
		constexpr BlockState DeadHornCoralBlock()
		{
			return 9514;
		}
	}
	namespace DeadHornCoralFan
	{
		constexpr BlockState DeadHornCoralFan()
		{
			return 9549;
		}
	}
	namespace DeadHornCoralWallFan
	{
		constexpr BlockState DeadHornCoralWallFan(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9593;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9595;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9597;
			else return 9599;
		}
		BlockState DeadHornCoralWallFan();
		eBlockFace Facing(BlockState Block);
	}
	namespace DeadTubeCoral
	{
		constexpr BlockState DeadTubeCoral()
		{
			return 9521;
		}
	}
	namespace DeadTubeCoralBlock
	{
		constexpr BlockState DeadTubeCoralBlock()
		{
			return 9510;
		}
	}
	namespace DeadTubeCoralFan
	{
		constexpr BlockState DeadTubeCoralFan()
		{
			return 9541;
		}
	}
	namespace DeadTubeCoralWallFan
	{
		constexpr BlockState DeadTubeCoralWallFan(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9561;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9563;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9565;
			else return 9567;
		}
		BlockState DeadTubeCoralWallFan();
		eBlockFace Facing(BlockState Block);
	}
	namespace DetectorRail
	{
		enum class Shape
		{
			NorthSouth,
			EastWest,
			AscendingEast,
			AscendingWest,
			AscendingNorth,
			AscendingSouth
		};
		constexpr BlockState DetectorRail(const bool Powered, const enum Shape Shape)
		{
			if (Powered)
				if (Shape == Shape::NorthSouth) return 1317;
				else if (Shape == Shape::EastWest) return 1318;
				else if (Shape == Shape::AscendingEast) return 1319;
				else if (Shape == Shape::AscendingWest) return 1320;
				else if (Shape == Shape::AscendingNorth) return 1321;
				else return 1322;
			else
				if (Shape == Shape::NorthSouth) return 1323;
				else if (Shape == Shape::EastWest) return 1324;
				else if (Shape == Shape::AscendingEast) return 1325;
				else if (Shape == Shape::AscendingWest) return 1326;
				else if (Shape == Shape::AscendingNorth) return 1327;
				else return 1328;
		}
		BlockState DetectorRail();
		bool Powered(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace DiamondBlock
	{
		constexpr BlockState DiamondBlock()
		{
			return 3355;
		}
	}
	namespace DiamondOre
	{
		constexpr BlockState DiamondOre()
		{
			return 3354;
		}
	}
	namespace Diorite
	{
		constexpr BlockState Diorite()
		{
			return 4;
		}
	}
	namespace DioriteSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState DioriteSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10862;
			else if (Type == Type::Bottom) return 10864;
			else return 10866;
		}
		BlockState DioriteSlab();
		enum Type Type(BlockState Block);
	}
	namespace DioriteStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState DioriteStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10710;
					else if (Shape == Shape::InnerLeft) return 10712;
					else if (Shape == Shape::InnerRight) return 10714;
					else if (Shape == Shape::OuterLeft) return 10716;
					else return 10718;
				else
					if (Shape == Shape::Straight) return 10720;
					else if (Shape == Shape::InnerLeft) return 10722;
					else if (Shape == Shape::InnerRight) return 10724;
					else if (Shape == Shape::OuterLeft) return 10726;
					else return 10728;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10730;
					else if (Shape == Shape::InnerLeft) return 10732;
					else if (Shape == Shape::InnerRight) return 10734;
					else if (Shape == Shape::OuterLeft) return 10736;
					else return 10738;
				else
					if (Shape == Shape::Straight) return 10740;
					else if (Shape == Shape::InnerLeft) return 10742;
					else if (Shape == Shape::InnerRight) return 10744;
					else if (Shape == Shape::OuterLeft) return 10746;
					else return 10748;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10750;
					else if (Shape == Shape::InnerLeft) return 10752;
					else if (Shape == Shape::InnerRight) return 10754;
					else if (Shape == Shape::OuterLeft) return 10756;
					else return 10758;
				else
					if (Shape == Shape::Straight) return 10760;
					else if (Shape == Shape::InnerLeft) return 10762;
					else if (Shape == Shape::InnerRight) return 10764;
					else if (Shape == Shape::OuterLeft) return 10766;
					else return 10768;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10770;
					else if (Shape == Shape::InnerLeft) return 10772;
					else if (Shape == Shape::InnerRight) return 10774;
					else if (Shape == Shape::OuterLeft) return 10776;
					else return 10778;
				else
					if (Shape == Shape::Straight) return 10780;
					else if (Shape == Shape::InnerLeft) return 10782;
					else if (Shape == Shape::InnerRight) return 10784;
					else if (Shape == Shape::OuterLeft) return 10786;
					else return 10788;
		}
		BlockState DioriteStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace DioriteWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState DioriteWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14434;
							else if (West == West::Low) return 14435;
							else return 14436;
						else
							if (West == West::None) return 14440;
							else if (West == West::Low) return 14441;
							else return 14442;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14446;
							else if (West == West::Low) return 14447;
							else return 14448;
						else
							if (West == West::None) return 14452;
							else if (West == West::Low) return 14453;
							else return 14454;
					else
						if (Up)
							if (West == West::None) return 14458;
							else if (West == West::Low) return 14459;
							else return 14460;
						else
							if (West == West::None) return 14464;
							else if (West == West::Low) return 14465;
							else return 14466;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14470;
							else if (West == West::Low) return 14471;
							else return 14472;
						else
							if (West == West::None) return 14476;
							else if (West == West::Low) return 14477;
							else return 14478;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14482;
							else if (West == West::Low) return 14483;
							else return 14484;
						else
							if (West == West::None) return 14488;
							else if (West == West::Low) return 14489;
							else return 14490;
					else
						if (Up)
							if (West == West::None) return 14494;
							else if (West == West::Low) return 14495;
							else return 14496;
						else
							if (West == West::None) return 14500;
							else if (West == West::Low) return 14501;
							else return 14502;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14506;
							else if (West == West::Low) return 14507;
							else return 14508;
						else
							if (West == West::None) return 14512;
							else if (West == West::Low) return 14513;
							else return 14514;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14518;
							else if (West == West::Low) return 14519;
							else return 14520;
						else
							if (West == West::None) return 14524;
							else if (West == West::Low) return 14525;
							else return 14526;
					else
						if (Up)
							if (West == West::None) return 14530;
							else if (West == West::Low) return 14531;
							else return 14532;
						else
							if (West == West::None) return 14536;
							else if (West == West::Low) return 14537;
							else return 14538;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14542;
							else if (West == West::Low) return 14543;
							else return 14544;
						else
							if (West == West::None) return 14548;
							else if (West == West::Low) return 14549;
							else return 14550;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14554;
							else if (West == West::Low) return 14555;
							else return 14556;
						else
							if (West == West::None) return 14560;
							else if (West == West::Low) return 14561;
							else return 14562;
					else
						if (Up)
							if (West == West::None) return 14566;
							else if (West == West::Low) return 14567;
							else return 14568;
						else
							if (West == West::None) return 14572;
							else if (West == West::Low) return 14573;
							else return 14574;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14578;
							else if (West == West::Low) return 14579;
							else return 14580;
						else
							if (West == West::None) return 14584;
							else if (West == West::Low) return 14585;
							else return 14586;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14590;
							else if (West == West::Low) return 14591;
							else return 14592;
						else
							if (West == West::None) return 14596;
							else if (West == West::Low) return 14597;
							else return 14598;
					else
						if (Up)
							if (West == West::None) return 14602;
							else if (West == West::Low) return 14603;
							else return 14604;
						else
							if (West == West::None) return 14608;
							else if (West == West::Low) return 14609;
							else return 14610;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14614;
							else if (West == West::Low) return 14615;
							else return 14616;
						else
							if (West == West::None) return 14620;
							else if (West == West::Low) return 14621;
							else return 14622;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14626;
							else if (West == West::Low) return 14627;
							else return 14628;
						else
							if (West == West::None) return 14632;
							else if (West == West::Low) return 14633;
							else return 14634;
					else
						if (Up)
							if (West == West::None) return 14638;
							else if (West == West::Low) return 14639;
							else return 14640;
						else
							if (West == West::None) return 14644;
							else if (West == West::Low) return 14645;
							else return 14646;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14650;
							else if (West == West::Low) return 14651;
							else return 14652;
						else
							if (West == West::None) return 14656;
							else if (West == West::Low) return 14657;
							else return 14658;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14662;
							else if (West == West::Low) return 14663;
							else return 14664;
						else
							if (West == West::None) return 14668;
							else if (West == West::Low) return 14669;
							else return 14670;
					else
						if (Up)
							if (West == West::None) return 14674;
							else if (West == West::Low) return 14675;
							else return 14676;
						else
							if (West == West::None) return 14680;
							else if (West == West::Low) return 14681;
							else return 14682;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14686;
							else if (West == West::Low) return 14687;
							else return 14688;
						else
							if (West == West::None) return 14692;
							else if (West == West::Low) return 14693;
							else return 14694;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14698;
							else if (West == West::Low) return 14699;
							else return 14700;
						else
							if (West == West::None) return 14704;
							else if (West == West::Low) return 14705;
							else return 14706;
					else
						if (Up)
							if (West == West::None) return 14710;
							else if (West == West::Low) return 14711;
							else return 14712;
						else
							if (West == West::None) return 14716;
							else if (West == West::Low) return 14717;
							else return 14718;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14722;
							else if (West == West::Low) return 14723;
							else return 14724;
						else
							if (West == West::None) return 14728;
							else if (West == West::Low) return 14729;
							else return 14730;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14734;
							else if (West == West::Low) return 14735;
							else return 14736;
						else
							if (West == West::None) return 14740;
							else if (West == West::Low) return 14741;
							else return 14742;
					else
						if (Up)
							if (West == West::None) return 14746;
							else if (West == West::Low) return 14747;
							else return 14748;
						else
							if (West == West::None) return 14752;
							else if (West == West::Low) return 14753;
							else return 14754;
		}
		BlockState DioriteWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace Dirt
	{
		constexpr BlockState Dirt()
		{
			return 10;
		}
	}
	namespace Dispenser
	{
		constexpr BlockState Dispenser(const eBlockFace Facing, const bool Triggered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Triggered) return 234;
				else return 235;
			else if (Facing == eBlockFace::BLOCK_FACE_XP)
				if (Triggered) return 236;
				else return 237;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Triggered) return 238;
				else return 239;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Triggered) return 240;
				else return 241;
			else if (Facing == eBlockFace::BLOCK_FACE_YP)
				if (Triggered) return 242;
				else return 243;
			else
				if (Triggered) return 244;
				else return 245;
		}
		BlockState Dispenser();
		eBlockFace Facing(BlockState Block);
		bool Triggered(BlockState Block);
	}
	namespace DragonEgg
	{
		constexpr BlockState DragonEgg()
		{
			return 5155;
		}
	}
	namespace DragonHead
	{
		constexpr BlockState DragonHead(const unsigned char Rotation)
		{
			if (Rotation == 0) return 6590;
			else if (Rotation == 1) return 6591;
			else if (Rotation == 2) return 6592;
			else if (Rotation == 3) return 6593;
			else if (Rotation == 4) return 6594;
			else if (Rotation == 5) return 6595;
			else if (Rotation == 6) return 6596;
			else if (Rotation == 7) return 6597;
			else if (Rotation == 8) return 6598;
			else if (Rotation == 9) return 6599;
			else if (Rotation == 10) return 6600;
			else if (Rotation == 11) return 6601;
			else if (Rotation == 12) return 6602;
			else if (Rotation == 13) return 6603;
			else if (Rotation == 14) return 6604;
			else return 6605;
		}
		BlockState DragonHead();
		unsigned char Rotation(BlockState Block);
	}
	namespace DragonWallHead
	{
		constexpr BlockState DragonWallHead(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6606;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6607;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6608;
			else return 6609;
		}
		BlockState DragonWallHead();
		eBlockFace Facing(BlockState Block);
	}
	namespace DriedKelpBlock
	{
		constexpr BlockState DriedKelpBlock()
		{
			return 9497;
		}
	}
	namespace Dropper
	{
		constexpr BlockState Dropper(const eBlockFace Facing, const bool Triggered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Triggered) return 6835;
				else return 6836;
			else if (Facing == eBlockFace::BLOCK_FACE_XP)
				if (Triggered) return 6837;
				else return 6838;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Triggered) return 6839;
				else return 6840;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Triggered) return 6841;
				else return 6842;
			else if (Facing == eBlockFace::BLOCK_FACE_YP)
				if (Triggered) return 6843;
				else return 6844;
			else
				if (Triggered) return 6845;
				else return 6846;
		}
		BlockState Dropper();
		eBlockFace Facing(BlockState Block);
		bool Triggered(BlockState Block);
	}
	namespace EmeraldBlock
	{
		constexpr BlockState EmeraldBlock()
		{
			return 5403;
		}
	}
	namespace EmeraldOre
	{
		constexpr BlockState EmeraldOre()
		{
			return 5250;
		}
	}
	namespace EnchantingTable
	{
		constexpr BlockState EnchantingTable()
		{
			return 5132;
		}
	}
	namespace EndGateway
	{
		constexpr BlockState EndGateway()
		{
			return 9224;
		}
	}
	namespace EndPortal
	{
		constexpr BlockState EndPortal()
		{
			return 5145;
		}
	}
	namespace EndPortalFrame
	{
		constexpr BlockState EndPortalFrame(const bool Eye, const eBlockFace Facing)
		{
			if (Eye)
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 5146;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 5147;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 5148;
				else return 5149;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 5150;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 5151;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 5152;
				else return 5153;
		}
		BlockState EndPortalFrame();
		bool Eye(BlockState Block);
		eBlockFace Facing(BlockState Block);
	}
	namespace EndRod
	{
		constexpr BlockState EndRod(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9058;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9059;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9060;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9061;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9062;
			else return 9063;
		}
		BlockState EndRod();
		eBlockFace Facing(BlockState Block);
	}
	namespace EndStone
	{
		constexpr BlockState EndStone()
		{
			return 5154;
		}
	}
	namespace EndStoneBrickSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState EndStoneBrickSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10820;
			else if (Type == Type::Bottom) return 10822;
			else return 10824;
		}
		BlockState EndStoneBrickSlab();
		enum Type Type(BlockState Block);
	}
	namespace EndStoneBrickStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState EndStoneBrickStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10070;
					else if (Shape == Shape::InnerLeft) return 10072;
					else if (Shape == Shape::InnerRight) return 10074;
					else if (Shape == Shape::OuterLeft) return 10076;
					else return 10078;
				else
					if (Shape == Shape::Straight) return 10080;
					else if (Shape == Shape::InnerLeft) return 10082;
					else if (Shape == Shape::InnerRight) return 10084;
					else if (Shape == Shape::OuterLeft) return 10086;
					else return 10088;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10090;
					else if (Shape == Shape::InnerLeft) return 10092;
					else if (Shape == Shape::InnerRight) return 10094;
					else if (Shape == Shape::OuterLeft) return 10096;
					else return 10098;
				else
					if (Shape == Shape::Straight) return 10100;
					else if (Shape == Shape::InnerLeft) return 10102;
					else if (Shape == Shape::InnerRight) return 10104;
					else if (Shape == Shape::OuterLeft) return 10106;
					else return 10108;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10110;
					else if (Shape == Shape::InnerLeft) return 10112;
					else if (Shape == Shape::InnerRight) return 10114;
					else if (Shape == Shape::OuterLeft) return 10116;
					else return 10118;
				else
					if (Shape == Shape::Straight) return 10120;
					else if (Shape == Shape::InnerLeft) return 10122;
					else if (Shape == Shape::InnerRight) return 10124;
					else if (Shape == Shape::OuterLeft) return 10126;
					else return 10128;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10130;
					else if (Shape == Shape::InnerLeft) return 10132;
					else if (Shape == Shape::InnerRight) return 10134;
					else if (Shape == Shape::OuterLeft) return 10136;
					else return 10138;
				else
					if (Shape == Shape::Straight) return 10140;
					else if (Shape == Shape::InnerLeft) return 10142;
					else if (Shape == Shape::InnerRight) return 10144;
					else if (Shape == Shape::OuterLeft) return 10146;
					else return 10148;
		}
		BlockState EndStoneBrickStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace EndStoneBrickWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState EndStoneBrickWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14110;
							else if (West == West::Low) return 14111;
							else return 14112;
						else
							if (West == West::None) return 14116;
							else if (West == West::Low) return 14117;
							else return 14118;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14122;
							else if (West == West::Low) return 14123;
							else return 14124;
						else
							if (West == West::None) return 14128;
							else if (West == West::Low) return 14129;
							else return 14130;
					else
						if (Up)
							if (West == West::None) return 14134;
							else if (West == West::Low) return 14135;
							else return 14136;
						else
							if (West == West::None) return 14140;
							else if (West == West::Low) return 14141;
							else return 14142;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14146;
							else if (West == West::Low) return 14147;
							else return 14148;
						else
							if (West == West::None) return 14152;
							else if (West == West::Low) return 14153;
							else return 14154;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14158;
							else if (West == West::Low) return 14159;
							else return 14160;
						else
							if (West == West::None) return 14164;
							else if (West == West::Low) return 14165;
							else return 14166;
					else
						if (Up)
							if (West == West::None) return 14170;
							else if (West == West::Low) return 14171;
							else return 14172;
						else
							if (West == West::None) return 14176;
							else if (West == West::Low) return 14177;
							else return 14178;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14182;
							else if (West == West::Low) return 14183;
							else return 14184;
						else
							if (West == West::None) return 14188;
							else if (West == West::Low) return 14189;
							else return 14190;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14194;
							else if (West == West::Low) return 14195;
							else return 14196;
						else
							if (West == West::None) return 14200;
							else if (West == West::Low) return 14201;
							else return 14202;
					else
						if (Up)
							if (West == West::None) return 14206;
							else if (West == West::Low) return 14207;
							else return 14208;
						else
							if (West == West::None) return 14212;
							else if (West == West::Low) return 14213;
							else return 14214;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14218;
							else if (West == West::Low) return 14219;
							else return 14220;
						else
							if (West == West::None) return 14224;
							else if (West == West::Low) return 14225;
							else return 14226;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14230;
							else if (West == West::Low) return 14231;
							else return 14232;
						else
							if (West == West::None) return 14236;
							else if (West == West::Low) return 14237;
							else return 14238;
					else
						if (Up)
							if (West == West::None) return 14242;
							else if (West == West::Low) return 14243;
							else return 14244;
						else
							if (West == West::None) return 14248;
							else if (West == West::Low) return 14249;
							else return 14250;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14254;
							else if (West == West::Low) return 14255;
							else return 14256;
						else
							if (West == West::None) return 14260;
							else if (West == West::Low) return 14261;
							else return 14262;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14266;
							else if (West == West::Low) return 14267;
							else return 14268;
						else
							if (West == West::None) return 14272;
							else if (West == West::Low) return 14273;
							else return 14274;
					else
						if (Up)
							if (West == West::None) return 14278;
							else if (West == West::Low) return 14279;
							else return 14280;
						else
							if (West == West::None) return 14284;
							else if (West == West::Low) return 14285;
							else return 14286;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14290;
							else if (West == West::Low) return 14291;
							else return 14292;
						else
							if (West == West::None) return 14296;
							else if (West == West::Low) return 14297;
							else return 14298;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14302;
							else if (West == West::Low) return 14303;
							else return 14304;
						else
							if (West == West::None) return 14308;
							else if (West == West::Low) return 14309;
							else return 14310;
					else
						if (Up)
							if (West == West::None) return 14314;
							else if (West == West::Low) return 14315;
							else return 14316;
						else
							if (West == West::None) return 14320;
							else if (West == West::Low) return 14321;
							else return 14322;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14326;
							else if (West == West::Low) return 14327;
							else return 14328;
						else
							if (West == West::None) return 14332;
							else if (West == West::Low) return 14333;
							else return 14334;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14338;
							else if (West == West::Low) return 14339;
							else return 14340;
						else
							if (West == West::None) return 14344;
							else if (West == West::Low) return 14345;
							else return 14346;
					else
						if (Up)
							if (West == West::None) return 14350;
							else if (West == West::Low) return 14351;
							else return 14352;
						else
							if (West == West::None) return 14356;
							else if (West == West::Low) return 14357;
							else return 14358;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14362;
							else if (West == West::Low) return 14363;
							else return 14364;
						else
							if (West == West::None) return 14368;
							else if (West == West::Low) return 14369;
							else return 14370;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14374;
							else if (West == West::Low) return 14375;
							else return 14376;
						else
							if (West == West::None) return 14380;
							else if (West == West::Low) return 14381;
							else return 14382;
					else
						if (Up)
							if (West == West::None) return 14386;
							else if (West == West::Low) return 14387;
							else return 14388;
						else
							if (West == West::None) return 14392;
							else if (West == West::Low) return 14393;
							else return 14394;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14398;
							else if (West == West::Low) return 14399;
							else return 14400;
						else
							if (West == West::None) return 14404;
							else if (West == West::Low) return 14405;
							else return 14406;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14410;
							else if (West == West::Low) return 14411;
							else return 14412;
						else
							if (West == West::None) return 14416;
							else if (West == West::Low) return 14417;
							else return 14418;
					else
						if (Up)
							if (West == West::None) return 14422;
							else if (West == West::Low) return 14423;
							else return 14424;
						else
							if (West == West::None) return 14428;
							else if (West == West::Low) return 14429;
							else return 14430;
		}
		BlockState EndStoneBrickWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace EndStoneBricks
	{
		constexpr BlockState EndStoneBricks()
		{
			return 9218;
		}
	}
	namespace EnderChest
	{
		constexpr BlockState EnderChest(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 5252;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 5254;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 5256;
			else return 5258;
		}
		BlockState EnderChest();
		eBlockFace Facing(BlockState Block);
	}
	namespace Farmland
	{
		constexpr BlockState Farmland(const unsigned char Moisture)
		{
			if (Moisture == 0) return 3365;
			else if (Moisture == 1) return 3366;
			else if (Moisture == 2) return 3367;
			else if (Moisture == 3) return 3368;
			else if (Moisture == 4) return 3369;
			else if (Moisture == 5) return 3370;
			else if (Moisture == 6) return 3371;
			else return 3372;
		}
		BlockState Farmland();
		unsigned char Moisture(BlockState Block);
	}
	namespace Fern
	{
		constexpr BlockState Fern()
		{
			return 1343;
		}
	}
	namespace Fire
	{
		constexpr BlockState Fire(const unsigned char Age, const bool East, const bool North, const bool South, const bool Up, const bool West)
		{
			if (Age == 0)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1440;
								else return 1441;
							else
								if (West) return 1442;
								else return 1443;
						else
							if (Up)
								if (West) return 1444;
								else return 1445;
							else
								if (West) return 1446;
								else return 1447;
					else
						if (South)
							if (Up)
								if (West) return 1448;
								else return 1449;
							else
								if (West) return 1450;
								else return 1451;
						else
							if (Up)
								if (West) return 1452;
								else return 1453;
							else
								if (West) return 1454;
								else return 1455;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1456;
								else return 1457;
							else
								if (West) return 1458;
								else return 1459;
						else
							if (Up)
								if (West) return 1460;
								else return 1461;
							else
								if (West) return 1462;
								else return 1463;
					else
						if (South)
							if (Up)
								if (West) return 1464;
								else return 1465;
							else
								if (West) return 1466;
								else return 1467;
						else
							if (Up)
								if (West) return 1468;
								else return 1469;
							else
								if (West) return 1470;
								else return 1471;
			else if (Age == 1)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1472;
								else return 1473;
							else
								if (West) return 1474;
								else return 1475;
						else
							if (Up)
								if (West) return 1476;
								else return 1477;
							else
								if (West) return 1478;
								else return 1479;
					else
						if (South)
							if (Up)
								if (West) return 1480;
								else return 1481;
							else
								if (West) return 1482;
								else return 1483;
						else
							if (Up)
								if (West) return 1484;
								else return 1485;
							else
								if (West) return 1486;
								else return 1487;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1488;
								else return 1489;
							else
								if (West) return 1490;
								else return 1491;
						else
							if (Up)
								if (West) return 1492;
								else return 1493;
							else
								if (West) return 1494;
								else return 1495;
					else
						if (South)
							if (Up)
								if (West) return 1496;
								else return 1497;
							else
								if (West) return 1498;
								else return 1499;
						else
							if (Up)
								if (West) return 1500;
								else return 1501;
							else
								if (West) return 1502;
								else return 1503;
			else if (Age == 2)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1504;
								else return 1505;
							else
								if (West) return 1506;
								else return 1507;
						else
							if (Up)
								if (West) return 1508;
								else return 1509;
							else
								if (West) return 1510;
								else return 1511;
					else
						if (South)
							if (Up)
								if (West) return 1512;
								else return 1513;
							else
								if (West) return 1514;
								else return 1515;
						else
							if (Up)
								if (West) return 1516;
								else return 1517;
							else
								if (West) return 1518;
								else return 1519;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1520;
								else return 1521;
							else
								if (West) return 1522;
								else return 1523;
						else
							if (Up)
								if (West) return 1524;
								else return 1525;
							else
								if (West) return 1526;
								else return 1527;
					else
						if (South)
							if (Up)
								if (West) return 1528;
								else return 1529;
							else
								if (West) return 1530;
								else return 1531;
						else
							if (Up)
								if (West) return 1532;
								else return 1533;
							else
								if (West) return 1534;
								else return 1535;
			else if (Age == 3)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1536;
								else return 1537;
							else
								if (West) return 1538;
								else return 1539;
						else
							if (Up)
								if (West) return 1540;
								else return 1541;
							else
								if (West) return 1542;
								else return 1543;
					else
						if (South)
							if (Up)
								if (West) return 1544;
								else return 1545;
							else
								if (West) return 1546;
								else return 1547;
						else
							if (Up)
								if (West) return 1548;
								else return 1549;
							else
								if (West) return 1550;
								else return 1551;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1552;
								else return 1553;
							else
								if (West) return 1554;
								else return 1555;
						else
							if (Up)
								if (West) return 1556;
								else return 1557;
							else
								if (West) return 1558;
								else return 1559;
					else
						if (South)
							if (Up)
								if (West) return 1560;
								else return 1561;
							else
								if (West) return 1562;
								else return 1563;
						else
							if (Up)
								if (West) return 1564;
								else return 1565;
							else
								if (West) return 1566;
								else return 1567;
			else if (Age == 4)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1568;
								else return 1569;
							else
								if (West) return 1570;
								else return 1571;
						else
							if (Up)
								if (West) return 1572;
								else return 1573;
							else
								if (West) return 1574;
								else return 1575;
					else
						if (South)
							if (Up)
								if (West) return 1576;
								else return 1577;
							else
								if (West) return 1578;
								else return 1579;
						else
							if (Up)
								if (West) return 1580;
								else return 1581;
							else
								if (West) return 1582;
								else return 1583;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1584;
								else return 1585;
							else
								if (West) return 1586;
								else return 1587;
						else
							if (Up)
								if (West) return 1588;
								else return 1589;
							else
								if (West) return 1590;
								else return 1591;
					else
						if (South)
							if (Up)
								if (West) return 1592;
								else return 1593;
							else
								if (West) return 1594;
								else return 1595;
						else
							if (Up)
								if (West) return 1596;
								else return 1597;
							else
								if (West) return 1598;
								else return 1599;
			else if (Age == 5)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1600;
								else return 1601;
							else
								if (West) return 1602;
								else return 1603;
						else
							if (Up)
								if (West) return 1604;
								else return 1605;
							else
								if (West) return 1606;
								else return 1607;
					else
						if (South)
							if (Up)
								if (West) return 1608;
								else return 1609;
							else
								if (West) return 1610;
								else return 1611;
						else
							if (Up)
								if (West) return 1612;
								else return 1613;
							else
								if (West) return 1614;
								else return 1615;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1616;
								else return 1617;
							else
								if (West) return 1618;
								else return 1619;
						else
							if (Up)
								if (West) return 1620;
								else return 1621;
							else
								if (West) return 1622;
								else return 1623;
					else
						if (South)
							if (Up)
								if (West) return 1624;
								else return 1625;
							else
								if (West) return 1626;
								else return 1627;
						else
							if (Up)
								if (West) return 1628;
								else return 1629;
							else
								if (West) return 1630;
								else return 1631;
			else if (Age == 6)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1632;
								else return 1633;
							else
								if (West) return 1634;
								else return 1635;
						else
							if (Up)
								if (West) return 1636;
								else return 1637;
							else
								if (West) return 1638;
								else return 1639;
					else
						if (South)
							if (Up)
								if (West) return 1640;
								else return 1641;
							else
								if (West) return 1642;
								else return 1643;
						else
							if (Up)
								if (West) return 1644;
								else return 1645;
							else
								if (West) return 1646;
								else return 1647;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1648;
								else return 1649;
							else
								if (West) return 1650;
								else return 1651;
						else
							if (Up)
								if (West) return 1652;
								else return 1653;
							else
								if (West) return 1654;
								else return 1655;
					else
						if (South)
							if (Up)
								if (West) return 1656;
								else return 1657;
							else
								if (West) return 1658;
								else return 1659;
						else
							if (Up)
								if (West) return 1660;
								else return 1661;
							else
								if (West) return 1662;
								else return 1663;
			else if (Age == 7)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1664;
								else return 1665;
							else
								if (West) return 1666;
								else return 1667;
						else
							if (Up)
								if (West) return 1668;
								else return 1669;
							else
								if (West) return 1670;
								else return 1671;
					else
						if (South)
							if (Up)
								if (West) return 1672;
								else return 1673;
							else
								if (West) return 1674;
								else return 1675;
						else
							if (Up)
								if (West) return 1676;
								else return 1677;
							else
								if (West) return 1678;
								else return 1679;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1680;
								else return 1681;
							else
								if (West) return 1682;
								else return 1683;
						else
							if (Up)
								if (West) return 1684;
								else return 1685;
							else
								if (West) return 1686;
								else return 1687;
					else
						if (South)
							if (Up)
								if (West) return 1688;
								else return 1689;
							else
								if (West) return 1690;
								else return 1691;
						else
							if (Up)
								if (West) return 1692;
								else return 1693;
							else
								if (West) return 1694;
								else return 1695;
			else if (Age == 8)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1696;
								else return 1697;
							else
								if (West) return 1698;
								else return 1699;
						else
							if (Up)
								if (West) return 1700;
								else return 1701;
							else
								if (West) return 1702;
								else return 1703;
					else
						if (South)
							if (Up)
								if (West) return 1704;
								else return 1705;
							else
								if (West) return 1706;
								else return 1707;
						else
							if (Up)
								if (West) return 1708;
								else return 1709;
							else
								if (West) return 1710;
								else return 1711;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1712;
								else return 1713;
							else
								if (West) return 1714;
								else return 1715;
						else
							if (Up)
								if (West) return 1716;
								else return 1717;
							else
								if (West) return 1718;
								else return 1719;
					else
						if (South)
							if (Up)
								if (West) return 1720;
								else return 1721;
							else
								if (West) return 1722;
								else return 1723;
						else
							if (Up)
								if (West) return 1724;
								else return 1725;
							else
								if (West) return 1726;
								else return 1727;
			else if (Age == 9)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1728;
								else return 1729;
							else
								if (West) return 1730;
								else return 1731;
						else
							if (Up)
								if (West) return 1732;
								else return 1733;
							else
								if (West) return 1734;
								else return 1735;
					else
						if (South)
							if (Up)
								if (West) return 1736;
								else return 1737;
							else
								if (West) return 1738;
								else return 1739;
						else
							if (Up)
								if (West) return 1740;
								else return 1741;
							else
								if (West) return 1742;
								else return 1743;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1744;
								else return 1745;
							else
								if (West) return 1746;
								else return 1747;
						else
							if (Up)
								if (West) return 1748;
								else return 1749;
							else
								if (West) return 1750;
								else return 1751;
					else
						if (South)
							if (Up)
								if (West) return 1752;
								else return 1753;
							else
								if (West) return 1754;
								else return 1755;
						else
							if (Up)
								if (West) return 1756;
								else return 1757;
							else
								if (West) return 1758;
								else return 1759;
			else if (Age == 10)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1760;
								else return 1761;
							else
								if (West) return 1762;
								else return 1763;
						else
							if (Up)
								if (West) return 1764;
								else return 1765;
							else
								if (West) return 1766;
								else return 1767;
					else
						if (South)
							if (Up)
								if (West) return 1768;
								else return 1769;
							else
								if (West) return 1770;
								else return 1771;
						else
							if (Up)
								if (West) return 1772;
								else return 1773;
							else
								if (West) return 1774;
								else return 1775;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1776;
								else return 1777;
							else
								if (West) return 1778;
								else return 1779;
						else
							if (Up)
								if (West) return 1780;
								else return 1781;
							else
								if (West) return 1782;
								else return 1783;
					else
						if (South)
							if (Up)
								if (West) return 1784;
								else return 1785;
							else
								if (West) return 1786;
								else return 1787;
						else
							if (Up)
								if (West) return 1788;
								else return 1789;
							else
								if (West) return 1790;
								else return 1791;
			else if (Age == 11)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1792;
								else return 1793;
							else
								if (West) return 1794;
								else return 1795;
						else
							if (Up)
								if (West) return 1796;
								else return 1797;
							else
								if (West) return 1798;
								else return 1799;
					else
						if (South)
							if (Up)
								if (West) return 1800;
								else return 1801;
							else
								if (West) return 1802;
								else return 1803;
						else
							if (Up)
								if (West) return 1804;
								else return 1805;
							else
								if (West) return 1806;
								else return 1807;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1808;
								else return 1809;
							else
								if (West) return 1810;
								else return 1811;
						else
							if (Up)
								if (West) return 1812;
								else return 1813;
							else
								if (West) return 1814;
								else return 1815;
					else
						if (South)
							if (Up)
								if (West) return 1816;
								else return 1817;
							else
								if (West) return 1818;
								else return 1819;
						else
							if (Up)
								if (West) return 1820;
								else return 1821;
							else
								if (West) return 1822;
								else return 1823;
			else if (Age == 12)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1824;
								else return 1825;
							else
								if (West) return 1826;
								else return 1827;
						else
							if (Up)
								if (West) return 1828;
								else return 1829;
							else
								if (West) return 1830;
								else return 1831;
					else
						if (South)
							if (Up)
								if (West) return 1832;
								else return 1833;
							else
								if (West) return 1834;
								else return 1835;
						else
							if (Up)
								if (West) return 1836;
								else return 1837;
							else
								if (West) return 1838;
								else return 1839;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1840;
								else return 1841;
							else
								if (West) return 1842;
								else return 1843;
						else
							if (Up)
								if (West) return 1844;
								else return 1845;
							else
								if (West) return 1846;
								else return 1847;
					else
						if (South)
							if (Up)
								if (West) return 1848;
								else return 1849;
							else
								if (West) return 1850;
								else return 1851;
						else
							if (Up)
								if (West) return 1852;
								else return 1853;
							else
								if (West) return 1854;
								else return 1855;
			else if (Age == 13)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1856;
								else return 1857;
							else
								if (West) return 1858;
								else return 1859;
						else
							if (Up)
								if (West) return 1860;
								else return 1861;
							else
								if (West) return 1862;
								else return 1863;
					else
						if (South)
							if (Up)
								if (West) return 1864;
								else return 1865;
							else
								if (West) return 1866;
								else return 1867;
						else
							if (Up)
								if (West) return 1868;
								else return 1869;
							else
								if (West) return 1870;
								else return 1871;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1872;
								else return 1873;
							else
								if (West) return 1874;
								else return 1875;
						else
							if (Up)
								if (West) return 1876;
								else return 1877;
							else
								if (West) return 1878;
								else return 1879;
					else
						if (South)
							if (Up)
								if (West) return 1880;
								else return 1881;
							else
								if (West) return 1882;
								else return 1883;
						else
							if (Up)
								if (West) return 1884;
								else return 1885;
							else
								if (West) return 1886;
								else return 1887;
			else if (Age == 14)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1888;
								else return 1889;
							else
								if (West) return 1890;
								else return 1891;
						else
							if (Up)
								if (West) return 1892;
								else return 1893;
							else
								if (West) return 1894;
								else return 1895;
					else
						if (South)
							if (Up)
								if (West) return 1896;
								else return 1897;
							else
								if (West) return 1898;
								else return 1899;
						else
							if (Up)
								if (West) return 1900;
								else return 1901;
							else
								if (West) return 1902;
								else return 1903;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1904;
								else return 1905;
							else
								if (West) return 1906;
								else return 1907;
						else
							if (Up)
								if (West) return 1908;
								else return 1909;
							else
								if (West) return 1910;
								else return 1911;
					else
						if (South)
							if (Up)
								if (West) return 1912;
								else return 1913;
							else
								if (West) return 1914;
								else return 1915;
						else
							if (Up)
								if (West) return 1916;
								else return 1917;
							else
								if (West) return 1918;
								else return 1919;
			else
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 1920;
								else return 1921;
							else
								if (West) return 1922;
								else return 1923;
						else
							if (Up)
								if (West) return 1924;
								else return 1925;
							else
								if (West) return 1926;
								else return 1927;
					else
						if (South)
							if (Up)
								if (West) return 1928;
								else return 1929;
							else
								if (West) return 1930;
								else return 1931;
						else
							if (Up)
								if (West) return 1932;
								else return 1933;
							else
								if (West) return 1934;
								else return 1935;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 1936;
								else return 1937;
							else
								if (West) return 1938;
								else return 1939;
						else
							if (Up)
								if (West) return 1940;
								else return 1941;
							else
								if (West) return 1942;
								else return 1943;
					else
						if (South)
							if (Up)
								if (West) return 1944;
								else return 1945;
							else
								if (West) return 1946;
								else return 1947;
						else
							if (Up)
								if (West) return 1948;
								else return 1949;
							else
								if (West) return 1950;
								else return 1951;
		}
		BlockState Fire();
		unsigned char Age(BlockState Block);
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool Up(BlockState Block);
		bool West(BlockState Block);
	}
	namespace FireCoral
	{
		constexpr BlockState FireCoral()
		{
			return 9537;
		}
	}
	namespace FireCoralBlock
	{
		constexpr BlockState FireCoralBlock()
		{
			return 9518;
		}
	}
	namespace FireCoralFan
	{
		constexpr BlockState FireCoralFan()
		{
			return 9557;
		}
	}
	namespace FireCoralWallFan
	{
		constexpr BlockState FireCoralWallFan(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9625;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9627;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9629;
			else return 9631;
		}
		BlockState FireCoralWallFan();
		eBlockFace Facing(BlockState Block);
	}
	namespace FletchingTable
	{
		constexpr BlockState FletchingTable()
		{
			return 14820;
		}
	}
	namespace FlowerPot
	{
		constexpr BlockState FlowerPot()
		{
			return 6305;
		}
	}
	namespace FrostedIce
	{
		constexpr BlockState FrostedIce(const unsigned char Age)
		{
			if (Age == 0) return 9249;
			else if (Age == 1) return 9250;
			else if (Age == 2) return 9251;
			else return 9252;
		}
		BlockState FrostedIce();
		unsigned char Age(BlockState Block);
	}
	namespace Furnace
	{
		constexpr BlockState Furnace(const eBlockFace Facing, const bool Lit)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Lit) return 3373;
				else return 3374;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Lit) return 3375;
				else return 3376;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Lit) return 3377;
				else return 3378;
			else
				if (Lit) return 3379;
				else return 3380;
		}
		BlockState Furnace();
		eBlockFace Facing(BlockState Block);
		bool Lit(BlockState Block);
	}
	namespace GildedBlackstone
	{
		constexpr BlockState GildedBlackstone()
		{
			return 16664;
		}
	}
	namespace Glass
	{
		constexpr BlockState Glass()
		{
			return 231;
		}
	}
	namespace GlassPane
	{
		constexpr BlockState GlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 4733;
						else return 4734;
					else
						if (West) return 4737;
						else return 4738;
				else
					if (South)
						if (West) return 4741;
						else return 4742;
					else
						if (West) return 4745;
						else return 4746;
			else
				if (North)
					if (South)
						if (West) return 4749;
						else return 4750;
					else
						if (West) return 4753;
						else return 4754;
				else
					if (South)
						if (West) return 4757;
						else return 4758;
					else
						if (West) return 4761;
						else return 4762;
		}
		BlockState GlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace Glowstone
	{
		constexpr BlockState Glowstone()
		{
			return 4013;
		}
	}
	namespace GoldBlock
	{
		constexpr BlockState GoldBlock()
		{
			return 1427;
		}
	}
	namespace GoldOre
	{
		constexpr BlockState GoldOre()
		{
			return 69;
		}
	}
	namespace Granite
	{
		constexpr BlockState Granite()
		{
			return 2;
		}
	}
	namespace GraniteSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState GraniteSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10838;
			else if (Type == Type::Bottom) return 10840;
			else return 10842;
		}
		BlockState GraniteSlab();
		enum Type Type(BlockState Block);
	}
	namespace GraniteStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState GraniteStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10390;
					else if (Shape == Shape::InnerLeft) return 10392;
					else if (Shape == Shape::InnerRight) return 10394;
					else if (Shape == Shape::OuterLeft) return 10396;
					else return 10398;
				else
					if (Shape == Shape::Straight) return 10400;
					else if (Shape == Shape::InnerLeft) return 10402;
					else if (Shape == Shape::InnerRight) return 10404;
					else if (Shape == Shape::OuterLeft) return 10406;
					else return 10408;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10410;
					else if (Shape == Shape::InnerLeft) return 10412;
					else if (Shape == Shape::InnerRight) return 10414;
					else if (Shape == Shape::OuterLeft) return 10416;
					else return 10418;
				else
					if (Shape == Shape::Straight) return 10420;
					else if (Shape == Shape::InnerLeft) return 10422;
					else if (Shape == Shape::InnerRight) return 10424;
					else if (Shape == Shape::OuterLeft) return 10426;
					else return 10428;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10430;
					else if (Shape == Shape::InnerLeft) return 10432;
					else if (Shape == Shape::InnerRight) return 10434;
					else if (Shape == Shape::OuterLeft) return 10436;
					else return 10438;
				else
					if (Shape == Shape::Straight) return 10440;
					else if (Shape == Shape::InnerLeft) return 10442;
					else if (Shape == Shape::InnerRight) return 10444;
					else if (Shape == Shape::OuterLeft) return 10446;
					else return 10448;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10450;
					else if (Shape == Shape::InnerLeft) return 10452;
					else if (Shape == Shape::InnerRight) return 10454;
					else if (Shape == Shape::OuterLeft) return 10456;
					else return 10458;
				else
					if (Shape == Shape::Straight) return 10460;
					else if (Shape == Shape::InnerLeft) return 10462;
					else if (Shape == Shape::InnerRight) return 10464;
					else if (Shape == Shape::OuterLeft) return 10466;
					else return 10468;
		}
		BlockState GraniteStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace GraniteWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState GraniteWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12166;
							else if (West == West::Low) return 12167;
							else return 12168;
						else
							if (West == West::None) return 12172;
							else if (West == West::Low) return 12173;
							else return 12174;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12178;
							else if (West == West::Low) return 12179;
							else return 12180;
						else
							if (West == West::None) return 12184;
							else if (West == West::Low) return 12185;
							else return 12186;
					else
						if (Up)
							if (West == West::None) return 12190;
							else if (West == West::Low) return 12191;
							else return 12192;
						else
							if (West == West::None) return 12196;
							else if (West == West::Low) return 12197;
							else return 12198;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12202;
							else if (West == West::Low) return 12203;
							else return 12204;
						else
							if (West == West::None) return 12208;
							else if (West == West::Low) return 12209;
							else return 12210;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12214;
							else if (West == West::Low) return 12215;
							else return 12216;
						else
							if (West == West::None) return 12220;
							else if (West == West::Low) return 12221;
							else return 12222;
					else
						if (Up)
							if (West == West::None) return 12226;
							else if (West == West::Low) return 12227;
							else return 12228;
						else
							if (West == West::None) return 12232;
							else if (West == West::Low) return 12233;
							else return 12234;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12238;
							else if (West == West::Low) return 12239;
							else return 12240;
						else
							if (West == West::None) return 12244;
							else if (West == West::Low) return 12245;
							else return 12246;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12250;
							else if (West == West::Low) return 12251;
							else return 12252;
						else
							if (West == West::None) return 12256;
							else if (West == West::Low) return 12257;
							else return 12258;
					else
						if (Up)
							if (West == West::None) return 12262;
							else if (West == West::Low) return 12263;
							else return 12264;
						else
							if (West == West::None) return 12268;
							else if (West == West::Low) return 12269;
							else return 12270;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12274;
							else if (West == West::Low) return 12275;
							else return 12276;
						else
							if (West == West::None) return 12280;
							else if (West == West::Low) return 12281;
							else return 12282;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12286;
							else if (West == West::Low) return 12287;
							else return 12288;
						else
							if (West == West::None) return 12292;
							else if (West == West::Low) return 12293;
							else return 12294;
					else
						if (Up)
							if (West == West::None) return 12298;
							else if (West == West::Low) return 12299;
							else return 12300;
						else
							if (West == West::None) return 12304;
							else if (West == West::Low) return 12305;
							else return 12306;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12310;
							else if (West == West::Low) return 12311;
							else return 12312;
						else
							if (West == West::None) return 12316;
							else if (West == West::Low) return 12317;
							else return 12318;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12322;
							else if (West == West::Low) return 12323;
							else return 12324;
						else
							if (West == West::None) return 12328;
							else if (West == West::Low) return 12329;
							else return 12330;
					else
						if (Up)
							if (West == West::None) return 12334;
							else if (West == West::Low) return 12335;
							else return 12336;
						else
							if (West == West::None) return 12340;
							else if (West == West::Low) return 12341;
							else return 12342;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12346;
							else if (West == West::Low) return 12347;
							else return 12348;
						else
							if (West == West::None) return 12352;
							else if (West == West::Low) return 12353;
							else return 12354;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12358;
							else if (West == West::Low) return 12359;
							else return 12360;
						else
							if (West == West::None) return 12364;
							else if (West == West::Low) return 12365;
							else return 12366;
					else
						if (Up)
							if (West == West::None) return 12370;
							else if (West == West::Low) return 12371;
							else return 12372;
						else
							if (West == West::None) return 12376;
							else if (West == West::Low) return 12377;
							else return 12378;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12382;
							else if (West == West::Low) return 12383;
							else return 12384;
						else
							if (West == West::None) return 12388;
							else if (West == West::Low) return 12389;
							else return 12390;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12394;
							else if (West == West::Low) return 12395;
							else return 12396;
						else
							if (West == West::None) return 12400;
							else if (West == West::Low) return 12401;
							else return 12402;
					else
						if (Up)
							if (West == West::None) return 12406;
							else if (West == West::Low) return 12407;
							else return 12408;
						else
							if (West == West::None) return 12412;
							else if (West == West::Low) return 12413;
							else return 12414;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12418;
							else if (West == West::Low) return 12419;
							else return 12420;
						else
							if (West == West::None) return 12424;
							else if (West == West::Low) return 12425;
							else return 12426;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12430;
							else if (West == West::Low) return 12431;
							else return 12432;
						else
							if (West == West::None) return 12436;
							else if (West == West::Low) return 12437;
							else return 12438;
					else
						if (Up)
							if (West == West::None) return 12442;
							else if (West == West::Low) return 12443;
							else return 12444;
						else
							if (West == West::None) return 12448;
							else if (West == West::Low) return 12449;
							else return 12450;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12454;
							else if (West == West::Low) return 12455;
							else return 12456;
						else
							if (West == West::None) return 12460;
							else if (West == West::Low) return 12461;
							else return 12462;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12466;
							else if (West == West::Low) return 12467;
							else return 12468;
						else
							if (West == West::None) return 12472;
							else if (West == West::Low) return 12473;
							else return 12474;
					else
						if (Up)
							if (West == West::None) return 12478;
							else if (West == West::Low) return 12479;
							else return 12480;
						else
							if (West == West::None) return 12484;
							else if (West == West::Low) return 12485;
							else return 12486;
		}
		BlockState GraniteWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace Grass
	{
		constexpr BlockState Grass()
		{
			return 1342;
		}
	}
	namespace GrassBlock
	{
		constexpr BlockState GrassBlock(const bool Snowy)
		{
			if (Snowy) return 8;
			else return 9;
		}
		BlockState GrassBlock();
		bool Snowy(BlockState Block);
	}
	namespace GrassPath
	{
		constexpr BlockState GrassPath()
		{
			return 9223;
		}
	}
	namespace Gravel
	{
		constexpr BlockState Gravel()
		{
			return 68;
		}
	}
	namespace GrayBanner
	{
		constexpr BlockState GrayBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 8009;
			else if (Rotation == 1) return 8010;
			else if (Rotation == 2) return 8011;
			else if (Rotation == 3) return 8012;
			else if (Rotation == 4) return 8013;
			else if (Rotation == 5) return 8014;
			else if (Rotation == 6) return 8015;
			else if (Rotation == 7) return 8016;
			else if (Rotation == 8) return 8017;
			else if (Rotation == 9) return 8018;
			else if (Rotation == 10) return 8019;
			else if (Rotation == 11) return 8020;
			else if (Rotation == 12) return 8021;
			else if (Rotation == 13) return 8022;
			else if (Rotation == 14) return 8023;
			else return 8024;
		}
		BlockState GrayBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace GrayBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState GrayBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1161;
					else return 1162;
				else
					if (Part == Part::Head) return 1163;
					else return 1164;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1165;
					else return 1166;
				else
					if (Part == Part::Head) return 1167;
					else return 1168;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1169;
					else return 1170;
				else
					if (Part == Part::Head) return 1171;
					else return 1172;
			else
				if (Occupied)
					if (Part == Part::Head) return 1173;
					else return 1174;
				else
					if (Part == Part::Head) return 1175;
					else return 1176;
		}
		BlockState GrayBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace GrayCarpet
	{
		constexpr BlockState GrayCarpet()
		{
			return 7873;
		}
	}
	namespace GrayConcrete
	{
		constexpr BlockState GrayConcrete()
		{
			return 9445;
		}
	}
	namespace GrayConcretePowder
	{
		constexpr BlockState GrayConcretePowder()
		{
			return 9461;
		}
	}
	namespace GrayGlazedTerracotta
	{
		constexpr BlockState GrayGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9402;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9403;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9404;
			else return 9405;
		}
		BlockState GrayGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace GrayShulkerBox
	{
		constexpr BlockState GrayShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9320;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9321;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9322;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9323;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9324;
			else return 9325;
		}
		BlockState GrayShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace GrayStainedGlass
	{
		constexpr BlockState GrayStainedGlass()
		{
			return 4102;
		}
	}
	namespace GrayStainedGlassPane
	{
		constexpr BlockState GrayStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7089;
						else return 7090;
					else
						if (West) return 7093;
						else return 7094;
				else
					if (South)
						if (West) return 7097;
						else return 7098;
					else
						if (West) return 7101;
						else return 7102;
			else
				if (North)
					if (South)
						if (West) return 7105;
						else return 7106;
					else
						if (West) return 7109;
						else return 7110;
				else
					if (South)
						if (West) return 7113;
						else return 7114;
					else
						if (West) return 7117;
						else return 7118;
		}
		BlockState GrayStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace GrayTerracotta
	{
		constexpr BlockState GrayTerracotta()
		{
			return 6854;
		}
	}
	namespace GrayWallBanner
	{
		constexpr BlockState GrayWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8181;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8182;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8183;
			else return 8184;
		}
		BlockState GrayWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace GrayWool
	{
		constexpr BlockState GrayWool()
		{
			return 1391;
		}
	}
	namespace GreenBanner
	{
		constexpr BlockState GreenBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 8105;
			else if (Rotation == 1) return 8106;
			else if (Rotation == 2) return 8107;
			else if (Rotation == 3) return 8108;
			else if (Rotation == 4) return 8109;
			else if (Rotation == 5) return 8110;
			else if (Rotation == 6) return 8111;
			else if (Rotation == 7) return 8112;
			else if (Rotation == 8) return 8113;
			else if (Rotation == 9) return 8114;
			else if (Rotation == 10) return 8115;
			else if (Rotation == 11) return 8116;
			else if (Rotation == 12) return 8117;
			else if (Rotation == 13) return 8118;
			else if (Rotation == 14) return 8119;
			else return 8120;
		}
		BlockState GreenBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace GreenBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState GreenBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1257;
					else return 1258;
				else
					if (Part == Part::Head) return 1259;
					else return 1260;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1261;
					else return 1262;
				else
					if (Part == Part::Head) return 1263;
					else return 1264;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1265;
					else return 1266;
				else
					if (Part == Part::Head) return 1267;
					else return 1268;
			else
				if (Occupied)
					if (Part == Part::Head) return 1269;
					else return 1270;
				else
					if (Part == Part::Head) return 1271;
					else return 1272;
		}
		BlockState GreenBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace GreenCarpet
	{
		constexpr BlockState GreenCarpet()
		{
			return 7879;
		}
	}
	namespace GreenConcrete
	{
		constexpr BlockState GreenConcrete()
		{
			return 9451;
		}
	}
	namespace GreenConcretePowder
	{
		constexpr BlockState GreenConcretePowder()
		{
			return 9467;
		}
	}
	namespace GreenGlazedTerracotta
	{
		constexpr BlockState GreenGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9426;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9427;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9428;
			else return 9429;
		}
		BlockState GreenGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace GreenShulkerBox
	{
		constexpr BlockState GreenShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9356;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9357;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9358;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9359;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9360;
			else return 9361;
		}
		BlockState GreenShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace GreenStainedGlass
	{
		constexpr BlockState GreenStainedGlass()
		{
			return 4108;
		}
	}
	namespace GreenStainedGlassPane
	{
		constexpr BlockState GreenStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7281;
						else return 7282;
					else
						if (West) return 7285;
						else return 7286;
				else
					if (South)
						if (West) return 7289;
						else return 7290;
					else
						if (West) return 7293;
						else return 7294;
			else
				if (North)
					if (South)
						if (West) return 7297;
						else return 7298;
					else
						if (West) return 7301;
						else return 7302;
				else
					if (South)
						if (West) return 7305;
						else return 7306;
					else
						if (West) return 7309;
						else return 7310;
		}
		BlockState GreenStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace GreenTerracotta
	{
		constexpr BlockState GreenTerracotta()
		{
			return 6860;
		}
	}
	namespace GreenWallBanner
	{
		constexpr BlockState GreenWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8205;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8206;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8207;
			else return 8208;
		}
		BlockState GreenWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace GreenWool
	{
		constexpr BlockState GreenWool()
		{
			return 1397;
		}
	}
	namespace Grindstone
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState Grindstone(const enum Face Face, const eBlockFace Facing)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 14821;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 14822;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 14823;
				else return 14824;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 14825;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 14826;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 14827;
				else return 14828;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 14829;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 14830;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 14831;
				else return 14832;
		}
		BlockState Grindstone();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
	}
	namespace HayBale
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState HayBale(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 7863;
			else if (Axis == Axis::Y) return 7864;
			else return 7865;
		}
		BlockState HayBale();
		enum Axis Axis(BlockState Block);
	}
	namespace HeavyWeightedPressurePlate
	{
		constexpr BlockState HeavyWeightedPressurePlate(const unsigned char Power)
		{
			if (Power == 0) return 6662;
			else if (Power == 1) return 6663;
			else if (Power == 2) return 6664;
			else if (Power == 3) return 6665;
			else if (Power == 4) return 6666;
			else if (Power == 5) return 6667;
			else if (Power == 6) return 6668;
			else if (Power == 7) return 6669;
			else if (Power == 8) return 6670;
			else if (Power == 9) return 6671;
			else if (Power == 10) return 6672;
			else if (Power == 11) return 6673;
			else if (Power == 12) return 6674;
			else if (Power == 13) return 6675;
			else if (Power == 14) return 6676;
			else return 6677;
		}
		BlockState HeavyWeightedPressurePlate();
		unsigned char Power(BlockState Block);
	}
	namespace HoneyBlock
	{
		constexpr BlockState HoneyBlock()
		{
			return 15824;
		}
	}
	namespace HoneycombBlock
	{
		constexpr BlockState HoneycombBlock()
		{
			return 15825;
		}
	}
	namespace Hopper
	{
		constexpr BlockState Hopper(const bool Enabled, const eBlockFace Facing)
		{
			if (Enabled)
				if (Facing == eBlockFace::BLOCK_FACE_YM) return 6728;
				else if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6729;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6730;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6731;
				else return 6732;
			else
				if (Facing == eBlockFace::BLOCK_FACE_YM) return 6733;
				else if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6734;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6735;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6736;
				else return 6737;
		}
		BlockState Hopper();
		bool Enabled(BlockState Block);
		eBlockFace Facing(BlockState Block);
	}
	namespace HornCoral
	{
		constexpr BlockState HornCoral()
		{
			return 9539;
		}
	}
	namespace HornCoralBlock
	{
		constexpr BlockState HornCoralBlock()
		{
			return 9519;
		}
	}
	namespace HornCoralFan
	{
		constexpr BlockState HornCoralFan()
		{
			return 9559;
		}
	}
	namespace HornCoralWallFan
	{
		constexpr BlockState HornCoralWallFan(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9633;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9635;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9637;
			else return 9639;
		}
		BlockState HornCoralWallFan();
		eBlockFace Facing(BlockState Block);
	}
	namespace Ice
	{
		constexpr BlockState Ice()
		{
			return 3929;
		}
	}
	namespace InfestedChiseledStoneBricks
	{
		constexpr BlockState InfestedChiseledStoneBricks()
		{
			return 4504;
		}
	}
	namespace InfestedCobblestone
	{
		constexpr BlockState InfestedCobblestone()
		{
			return 4500;
		}
	}
	namespace InfestedCrackedStoneBricks
	{
		constexpr BlockState InfestedCrackedStoneBricks()
		{
			return 4503;
		}
	}
	namespace InfestedMossyStoneBricks
	{
		constexpr BlockState InfestedMossyStoneBricks()
		{
			return 4502;
		}
	}
	namespace InfestedStone
	{
		constexpr BlockState InfestedStone()
		{
			return 4499;
		}
	}
	namespace InfestedStoneBricks
	{
		constexpr BlockState InfestedStoneBricks()
		{
			return 4501;
		}
	}
	namespace IronBars
	{
		constexpr BlockState IronBars(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 4699;
						else return 4700;
					else
						if (West) return 4703;
						else return 4704;
				else
					if (South)
						if (West) return 4707;
						else return 4708;
					else
						if (West) return 4711;
						else return 4712;
			else
				if (North)
					if (South)
						if (West) return 4715;
						else return 4716;
					else
						if (West) return 4719;
						else return 4720;
				else
					if (South)
						if (West) return 4723;
						else return 4724;
					else
						if (West) return 4727;
						else return 4728;
		}
		BlockState IronBars();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace IronBlock
	{
		constexpr BlockState IronBlock()
		{
			return 1428;
		}
	}
	namespace IronDoor
	{
		enum class Half
		{
			Upper,
			Lower
		};
		enum class Hinge
		{
			Left,
			Right
		};
		constexpr BlockState IronDoor(const eBlockFace Facing, const enum Half Half, const enum Hinge Hinge, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3809;
							else return 3810;
						else
							if (Powered) return 3811;
							else return 3812;
					else
						if (Open)
							if (Powered) return 3813;
							else return 3814;
						else
							if (Powered) return 3815;
							else return 3816;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3817;
							else return 3818;
						else
							if (Powered) return 3819;
							else return 3820;
					else
						if (Open)
							if (Powered) return 3821;
							else return 3822;
						else
							if (Powered) return 3823;
							else return 3824;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3825;
							else return 3826;
						else
							if (Powered) return 3827;
							else return 3828;
					else
						if (Open)
							if (Powered) return 3829;
							else return 3830;
						else
							if (Powered) return 3831;
							else return 3832;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3833;
							else return 3834;
						else
							if (Powered) return 3835;
							else return 3836;
					else
						if (Open)
							if (Powered) return 3837;
							else return 3838;
						else
							if (Powered) return 3839;
							else return 3840;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3841;
							else return 3842;
						else
							if (Powered) return 3843;
							else return 3844;
					else
						if (Open)
							if (Powered) return 3845;
							else return 3846;
						else
							if (Powered) return 3847;
							else return 3848;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3849;
							else return 3850;
						else
							if (Powered) return 3851;
							else return 3852;
					else
						if (Open)
							if (Powered) return 3853;
							else return 3854;
						else
							if (Powered) return 3855;
							else return 3856;
			else
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3857;
							else return 3858;
						else
							if (Powered) return 3859;
							else return 3860;
					else
						if (Open)
							if (Powered) return 3861;
							else return 3862;
						else
							if (Powered) return 3863;
							else return 3864;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3865;
							else return 3866;
						else
							if (Powered) return 3867;
							else return 3868;
					else
						if (Open)
							if (Powered) return 3869;
							else return 3870;
						else
							if (Powered) return 3871;
							else return 3872;
		}
		BlockState IronDoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Hinge Hinge(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace IronOre
	{
		constexpr BlockState IronOre()
		{
			return 70;
		}
	}
	namespace IronTrapdoor
	{
		enum class Half
		{
			Top,
			Bottom
		};
		constexpr BlockState IronTrapdoor(const eBlockFace Facing, const enum Half Half, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 7538;
						else return 7540;
					else
						if (Powered) return 7542;
						else return 7544;
				else
					if (Open)
						if (Powered) return 7546;
						else return 7548;
					else
						if (Powered) return 7550;
						else return 7552;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 7554;
						else return 7556;
					else
						if (Powered) return 7558;
						else return 7560;
				else
					if (Open)
						if (Powered) return 7562;
						else return 7564;
					else
						if (Powered) return 7566;
						else return 7568;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 7570;
						else return 7572;
					else
						if (Powered) return 7574;
						else return 7576;
				else
					if (Open)
						if (Powered) return 7578;
						else return 7580;
					else
						if (Powered) return 7582;
						else return 7584;
			else
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 7586;
						else return 7588;
					else
						if (Powered) return 7590;
						else return 7592;
				else
					if (Open)
						if (Powered) return 7594;
						else return 7596;
					else
						if (Powered) return 7598;
						else return 7600;
		}
		BlockState IronTrapdoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace JackOLantern
	{
		constexpr BlockState JackOLantern(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 4020;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 4021;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 4022;
			else return 4023;
		}
		BlockState JackOLantern();
		eBlockFace Facing(BlockState Block);
	}
	namespace Jigsaw
	{
		enum class Orientation
		{
			DownEast,
			DownNorth,
			DownSouth,
			DownWest,
			UpEast,
			UpNorth,
			UpSouth,
			UpWest,
			WestUp,
			EastUp,
			NorthUp,
			SouthUp
		};
		constexpr BlockState Jigsaw(const enum Orientation Orientation)
		{
			if (Orientation == Orientation::DownEast) return 15739;
			else if (Orientation == Orientation::DownNorth) return 15740;
			else if (Orientation == Orientation::DownSouth) return 15741;
			else if (Orientation == Orientation::DownWest) return 15742;
			else if (Orientation == Orientation::UpEast) return 15743;
			else if (Orientation == Orientation::UpNorth) return 15744;
			else if (Orientation == Orientation::UpSouth) return 15745;
			else if (Orientation == Orientation::UpWest) return 15746;
			else if (Orientation == Orientation::WestUp) return 15747;
			else if (Orientation == Orientation::EastUp) return 15748;
			else if (Orientation == Orientation::NorthUp) return 15749;
			else return 15750;
		}
		BlockState Jigsaw();
		enum Orientation Orientation(BlockState Block);
	}
	namespace Jukebox
	{
		constexpr BlockState Jukebox(const bool HasRecord)
		{
			if (HasRecord) return 3964;
			else return 3965;
		}
		BlockState Jukebox();
		bool HasRecord(BlockState Block);
	}
	namespace JungleButton
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState JungleButton(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6418;
					else return 6419;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6420;
					else return 6421;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6422;
					else return 6423;
				else
					if (Powered) return 6424;
					else return 6425;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6426;
					else return 6427;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6428;
					else return 6429;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6430;
					else return 6431;
				else
					if (Powered) return 6432;
					else return 6433;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6434;
					else return 6435;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6436;
					else return 6437;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6438;
					else return 6439;
				else
					if (Powered) return 6440;
					else return 6441;
		}
		BlockState JungleButton();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace JungleDoor
	{
		enum class Half
		{
			Upper,
			Lower
		};
		enum class Hinge
		{
			Left,
			Right
		};
		constexpr BlockState JungleDoor(const eBlockFace Facing, const enum Half Half, const enum Hinge Hinge, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8866;
							else return 8867;
						else
							if (Powered) return 8868;
							else return 8869;
					else
						if (Open)
							if (Powered) return 8870;
							else return 8871;
						else
							if (Powered) return 8872;
							else return 8873;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8874;
							else return 8875;
						else
							if (Powered) return 8876;
							else return 8877;
					else
						if (Open)
							if (Powered) return 8878;
							else return 8879;
						else
							if (Powered) return 8880;
							else return 8881;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8882;
							else return 8883;
						else
							if (Powered) return 8884;
							else return 8885;
					else
						if (Open)
							if (Powered) return 8886;
							else return 8887;
						else
							if (Powered) return 8888;
							else return 8889;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8890;
							else return 8891;
						else
							if (Powered) return 8892;
							else return 8893;
					else
						if (Open)
							if (Powered) return 8894;
							else return 8895;
						else
							if (Powered) return 8896;
							else return 8897;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8898;
							else return 8899;
						else
							if (Powered) return 8900;
							else return 8901;
					else
						if (Open)
							if (Powered) return 8902;
							else return 8903;
						else
							if (Powered) return 8904;
							else return 8905;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8906;
							else return 8907;
						else
							if (Powered) return 8908;
							else return 8909;
					else
						if (Open)
							if (Powered) return 8910;
							else return 8911;
						else
							if (Powered) return 8912;
							else return 8913;
			else
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8914;
							else return 8915;
						else
							if (Powered) return 8916;
							else return 8917;
					else
						if (Open)
							if (Powered) return 8918;
							else return 8919;
						else
							if (Powered) return 8920;
							else return 8921;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8922;
							else return 8923;
						else
							if (Powered) return 8924;
							else return 8925;
					else
						if (Open)
							if (Powered) return 8926;
							else return 8927;
						else
							if (Powered) return 8928;
							else return 8929;
		}
		BlockState JungleDoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Hinge Hinge(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace JungleFence
	{
		constexpr BlockState JungleFence(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 8644;
						else return 8645;
					else
						if (West) return 8648;
						else return 8649;
				else
					if (South)
						if (West) return 8652;
						else return 8653;
					else
						if (West) return 8656;
						else return 8657;
			else
				if (North)
					if (South)
						if (West) return 8660;
						else return 8661;
					else
						if (West) return 8664;
						else return 8665;
				else
					if (South)
						if (West) return 8668;
						else return 8669;
					else
						if (West) return 8672;
						else return 8673;
		}
		BlockState JungleFence();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace JungleFenceGate
	{
		constexpr BlockState JungleFenceGate(const eBlockFace Facing, const bool InWall, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (InWall)
					if (Open)
						if (Powered) return 8482;
						else return 8483;
					else
						if (Powered) return 8484;
						else return 8485;
				else
					if (Open)
						if (Powered) return 8486;
						else return 8487;
					else
						if (Powered) return 8488;
						else return 8489;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (InWall)
					if (Open)
						if (Powered) return 8490;
						else return 8491;
					else
						if (Powered) return 8492;
						else return 8493;
				else
					if (Open)
						if (Powered) return 8494;
						else return 8495;
					else
						if (Powered) return 8496;
						else return 8497;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (InWall)
					if (Open)
						if (Powered) return 8498;
						else return 8499;
					else
						if (Powered) return 8500;
						else return 8501;
				else
					if (Open)
						if (Powered) return 8502;
						else return 8503;
					else
						if (Powered) return 8504;
						else return 8505;
			else
				if (InWall)
					if (Open)
						if (Powered) return 8506;
						else return 8507;
					else
						if (Powered) return 8508;
						else return 8509;
				else
					if (Open)
						if (Powered) return 8510;
						else return 8511;
					else
						if (Powered) return 8512;
						else return 8513;
		}
		BlockState JungleFenceGate();
		eBlockFace Facing(BlockState Block);
		bool InWall(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace JungleLeaves
	{
		constexpr BlockState JungleLeaves(const unsigned char Distance, const bool Persistent)
		{
			if (Distance == 1)
				if (Persistent) return 187;
				else return 188;
			else if (Distance == 2)
				if (Persistent) return 189;
				else return 190;
			else if (Distance == 3)
				if (Persistent) return 191;
				else return 192;
			else if (Distance == 4)
				if (Persistent) return 193;
				else return 194;
			else if (Distance == 5)
				if (Persistent) return 195;
				else return 196;
			else if (Distance == 6)
				if (Persistent) return 197;
				else return 198;
			else
				if (Persistent) return 199;
				else return 200;
		}
		BlockState JungleLeaves();
		unsigned char Distance(BlockState Block);
		bool Persistent(BlockState Block);
	}
	namespace JungleLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState JungleLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 82;
			else if (Axis == Axis::Y) return 83;
			else return 84;
		}
		BlockState JungleLog();
		enum Axis Axis(BlockState Block);
	}
	namespace JunglePlanks
	{
		constexpr BlockState JunglePlanks()
		{
			return 18;
		}
	}
	namespace JunglePressurePlate
	{
		constexpr BlockState JunglePressurePlate(const bool Powered)
		{
			if (Powered) return 3879;
			else return 3880;
		}
		BlockState JunglePressurePlate();
		bool Powered(BlockState Block);
	}
	namespace JungleSapling
	{
		constexpr BlockState JungleSapling(const unsigned char Stage)
		{
			if (Stage == 0) return 27;
			else return 28;
		}
		BlockState JungleSapling();
		unsigned char Stage(BlockState Block);
	}
	namespace JungleSign
	{
		constexpr BlockState JungleSign(const unsigned char Rotation)
		{
			if (Rotation == 0) return 3510;
			else if (Rotation == 1) return 3512;
			else if (Rotation == 2) return 3514;
			else if (Rotation == 3) return 3516;
			else if (Rotation == 4) return 3518;
			else if (Rotation == 5) return 3520;
			else if (Rotation == 6) return 3522;
			else if (Rotation == 7) return 3524;
			else if (Rotation == 8) return 3526;
			else if (Rotation == 9) return 3528;
			else if (Rotation == 10) return 3530;
			else if (Rotation == 11) return 3532;
			else if (Rotation == 12) return 3534;
			else if (Rotation == 13) return 3536;
			else if (Rotation == 14) return 3538;
			else return 3540;
		}
		BlockState JungleSign();
		unsigned char Rotation(BlockState Block);
	}
	namespace JungleSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState JungleSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8319;
			else if (Type == Type::Bottom) return 8321;
			else return 8323;
		}
		BlockState JungleSlab();
		enum Type Type(BlockState Block);
	}
	namespace JungleStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState JungleStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5565;
					else if (Shape == Shape::InnerLeft) return 5567;
					else if (Shape == Shape::InnerRight) return 5569;
					else if (Shape == Shape::OuterLeft) return 5571;
					else return 5573;
				else
					if (Shape == Shape::Straight) return 5575;
					else if (Shape == Shape::InnerLeft) return 5577;
					else if (Shape == Shape::InnerRight) return 5579;
					else if (Shape == Shape::OuterLeft) return 5581;
					else return 5583;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5585;
					else if (Shape == Shape::InnerLeft) return 5587;
					else if (Shape == Shape::InnerRight) return 5589;
					else if (Shape == Shape::OuterLeft) return 5591;
					else return 5593;
				else
					if (Shape == Shape::Straight) return 5595;
					else if (Shape == Shape::InnerLeft) return 5597;
					else if (Shape == Shape::InnerRight) return 5599;
					else if (Shape == Shape::OuterLeft) return 5601;
					else return 5603;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5605;
					else if (Shape == Shape::InnerLeft) return 5607;
					else if (Shape == Shape::InnerRight) return 5609;
					else if (Shape == Shape::OuterLeft) return 5611;
					else return 5613;
				else
					if (Shape == Shape::Straight) return 5615;
					else if (Shape == Shape::InnerLeft) return 5617;
					else if (Shape == Shape::InnerRight) return 5619;
					else if (Shape == Shape::OuterLeft) return 5621;
					else return 5623;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5625;
					else if (Shape == Shape::InnerLeft) return 5627;
					else if (Shape == Shape::InnerRight) return 5629;
					else if (Shape == Shape::OuterLeft) return 5631;
					else return 5633;
				else
					if (Shape == Shape::Straight) return 5635;
					else if (Shape == Shape::InnerLeft) return 5637;
					else if (Shape == Shape::InnerRight) return 5639;
					else if (Shape == Shape::OuterLeft) return 5641;
					else return 5643;
		}
		BlockState JungleStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace JungleTrapdoor
	{
		enum class Half
		{
			Top,
			Bottom
		};
		constexpr BlockState JungleTrapdoor(const eBlockFace Facing, const enum Half Half, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4304;
						else return 4306;
					else
						if (Powered) return 4308;
						else return 4310;
				else
					if (Open)
						if (Powered) return 4312;
						else return 4314;
					else
						if (Powered) return 4316;
						else return 4318;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4320;
						else return 4322;
					else
						if (Powered) return 4324;
						else return 4326;
				else
					if (Open)
						if (Powered) return 4328;
						else return 4330;
					else
						if (Powered) return 4332;
						else return 4334;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4336;
						else return 4338;
					else
						if (Powered) return 4340;
						else return 4342;
				else
					if (Open)
						if (Powered) return 4344;
						else return 4346;
					else
						if (Powered) return 4348;
						else return 4350;
			else
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4352;
						else return 4354;
					else
						if (Powered) return 4356;
						else return 4358;
				else
					if (Open)
						if (Powered) return 4360;
						else return 4362;
					else
						if (Powered) return 4364;
						else return 4366;
		}
		BlockState JungleTrapdoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace JungleWallSign
	{
		constexpr BlockState JungleWallSign(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 3768;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 3770;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 3772;
			else return 3774;
		}
		BlockState JungleWallSign();
		eBlockFace Facing(BlockState Block);
	}
	namespace JungleWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState JungleWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 118;
			else if (Axis == Axis::Y) return 119;
			else return 120;
		}
		BlockState JungleWood();
		enum Axis Axis(BlockState Block);
	}
	namespace Kelp
	{
		constexpr BlockState Kelp(const unsigned char Age)
		{
			if (Age == 0) return 9470;
			else if (Age == 1) return 9471;
			else if (Age == 2) return 9472;
			else if (Age == 3) return 9473;
			else if (Age == 4) return 9474;
			else if (Age == 5) return 9475;
			else if (Age == 6) return 9476;
			else if (Age == 7) return 9477;
			else if (Age == 8) return 9478;
			else if (Age == 9) return 9479;
			else if (Age == 10) return 9480;
			else if (Age == 11) return 9481;
			else if (Age == 12) return 9482;
			else if (Age == 13) return 9483;
			else if (Age == 14) return 9484;
			else if (Age == 15) return 9485;
			else if (Age == 16) return 9486;
			else if (Age == 17) return 9487;
			else if (Age == 18) return 9488;
			else if (Age == 19) return 9489;
			else if (Age == 20) return 9490;
			else if (Age == 21) return 9491;
			else if (Age == 22) return 9492;
			else if (Age == 23) return 9493;
			else if (Age == 24) return 9494;
			else return 9495;
		}
		BlockState Kelp();
		unsigned char Age(BlockState Block);
	}
	namespace KelpPlant
	{
		constexpr BlockState KelpPlant()
		{
			return 9496;
		}
	}
	namespace Ladder
	{
		constexpr BlockState Ladder(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 3638;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 3640;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 3642;
			else return 3644;
		}
		BlockState Ladder();
		eBlockFace Facing(BlockState Block);
	}
	namespace Lantern
	{
		constexpr BlockState Lantern(const bool Hanging)
		{
			if (Hanging) return 14886;
			else return 14887;
		}
		BlockState Lantern();
		bool Hanging(BlockState Block);
	}
	namespace LapisBlock
	{
		constexpr BlockState LapisBlock()
		{
			return 233;
		}
	}
	namespace LapisOre
	{
		constexpr BlockState LapisOre()
		{
			return 232;
		}
	}
	namespace LargeFern
	{
		enum class Half
		{
			Upper,
			Lower
		};
		constexpr BlockState LargeFern(const enum Half Half)
		{
			if (Half == Half::Upper) return 7895;
			else return 7896;
		}
		BlockState LargeFern();
		enum Half Half(BlockState Block);
	}
	namespace Lava
	{
		constexpr BlockState Lava(const unsigned char Level)
		{
			if (Level == 0) return 50;
			else if (Level == 1) return 51;
			else if (Level == 2) return 52;
			else if (Level == 3) return 53;
			else if (Level == 4) return 54;
			else if (Level == 5) return 55;
			else if (Level == 6) return 56;
			else if (Level == 7) return 57;
			else if (Level == 8) return 58;
			else if (Level == 9) return 59;
			else if (Level == 10) return 60;
			else if (Level == 11) return 61;
			else if (Level == 12) return 62;
			else if (Level == 13) return 63;
			else if (Level == 14) return 64;
			else return 65;
		}
		BlockState Lava();
		unsigned char Level(BlockState Block);
	}
	namespace Lectern
	{
		constexpr BlockState Lectern(const eBlockFace Facing, const bool HasBook, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (HasBook)
					if (Powered) return 14833;
					else return 14834;
				else
					if (Powered) return 14835;
					else return 14836;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (HasBook)
					if (Powered) return 14837;
					else return 14838;
				else
					if (Powered) return 14839;
					else return 14840;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (HasBook)
					if (Powered) return 14841;
					else return 14842;
				else
					if (Powered) return 14843;
					else return 14844;
			else
				if (HasBook)
					if (Powered) return 14845;
					else return 14846;
				else
					if (Powered) return 14847;
					else return 14848;
		}
		BlockState Lectern();
		eBlockFace Facing(BlockState Block);
		bool HasBook(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace Lever
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState Lever(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 3783;
					else return 3784;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 3785;
					else return 3786;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 3787;
					else return 3788;
				else
					if (Powered) return 3789;
					else return 3790;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 3791;
					else return 3792;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 3793;
					else return 3794;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 3795;
					else return 3796;
				else
					if (Powered) return 3797;
					else return 3798;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 3799;
					else return 3800;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 3801;
					else return 3802;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 3803;
					else return 3804;
				else
					if (Powered) return 3805;
					else return 3806;
		}
		BlockState Lever();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace LightBlueBanner
	{
		constexpr BlockState LightBlueBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 7945;
			else if (Rotation == 1) return 7946;
			else if (Rotation == 2) return 7947;
			else if (Rotation == 3) return 7948;
			else if (Rotation == 4) return 7949;
			else if (Rotation == 5) return 7950;
			else if (Rotation == 6) return 7951;
			else if (Rotation == 7) return 7952;
			else if (Rotation == 8) return 7953;
			else if (Rotation == 9) return 7954;
			else if (Rotation == 10) return 7955;
			else if (Rotation == 11) return 7956;
			else if (Rotation == 12) return 7957;
			else if (Rotation == 13) return 7958;
			else if (Rotation == 14) return 7959;
			else return 7960;
		}
		BlockState LightBlueBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace LightBlueBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState LightBlueBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1097;
					else return 1098;
				else
					if (Part == Part::Head) return 1099;
					else return 1100;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1101;
					else return 1102;
				else
					if (Part == Part::Head) return 1103;
					else return 1104;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1105;
					else return 1106;
				else
					if (Part == Part::Head) return 1107;
					else return 1108;
			else
				if (Occupied)
					if (Part == Part::Head) return 1109;
					else return 1110;
				else
					if (Part == Part::Head) return 1111;
					else return 1112;
		}
		BlockState LightBlueBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace LightBlueCarpet
	{
		constexpr BlockState LightBlueCarpet()
		{
			return 7869;
		}
	}
	namespace LightBlueConcrete
	{
		constexpr BlockState LightBlueConcrete()
		{
			return 9441;
		}
	}
	namespace LightBlueConcretePowder
	{
		constexpr BlockState LightBlueConcretePowder()
		{
			return 9457;
		}
	}
	namespace LightBlueGlazedTerracotta
	{
		constexpr BlockState LightBlueGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9386;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9387;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9388;
			else return 9389;
		}
		BlockState LightBlueGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace LightBlueShulkerBox
	{
		constexpr BlockState LightBlueShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9296;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9297;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9298;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9299;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9300;
			else return 9301;
		}
		BlockState LightBlueShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace LightBlueStainedGlass
	{
		constexpr BlockState LightBlueStainedGlass()
		{
			return 4098;
		}
	}
	namespace LightBlueStainedGlassPane
	{
		constexpr BlockState LightBlueStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 6961;
						else return 6962;
					else
						if (West) return 6965;
						else return 6966;
				else
					if (South)
						if (West) return 6969;
						else return 6970;
					else
						if (West) return 6973;
						else return 6974;
			else
				if (North)
					if (South)
						if (West) return 6977;
						else return 6978;
					else
						if (West) return 6981;
						else return 6982;
				else
					if (South)
						if (West) return 6985;
						else return 6986;
					else
						if (West) return 6989;
						else return 6990;
		}
		BlockState LightBlueStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace LightBlueTerracotta
	{
		constexpr BlockState LightBlueTerracotta()
		{
			return 6850;
		}
	}
	namespace LightBlueWallBanner
	{
		constexpr BlockState LightBlueWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8165;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8166;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8167;
			else return 8168;
		}
		BlockState LightBlueWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace LightBlueWool
	{
		constexpr BlockState LightBlueWool()
		{
			return 1387;
		}
	}
	namespace LightGrayBanner
	{
		constexpr BlockState LightGrayBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 8025;
			else if (Rotation == 1) return 8026;
			else if (Rotation == 2) return 8027;
			else if (Rotation == 3) return 8028;
			else if (Rotation == 4) return 8029;
			else if (Rotation == 5) return 8030;
			else if (Rotation == 6) return 8031;
			else if (Rotation == 7) return 8032;
			else if (Rotation == 8) return 8033;
			else if (Rotation == 9) return 8034;
			else if (Rotation == 10) return 8035;
			else if (Rotation == 11) return 8036;
			else if (Rotation == 12) return 8037;
			else if (Rotation == 13) return 8038;
			else if (Rotation == 14) return 8039;
			else return 8040;
		}
		BlockState LightGrayBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace LightGrayBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState LightGrayBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1177;
					else return 1178;
				else
					if (Part == Part::Head) return 1179;
					else return 1180;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1181;
					else return 1182;
				else
					if (Part == Part::Head) return 1183;
					else return 1184;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1185;
					else return 1186;
				else
					if (Part == Part::Head) return 1187;
					else return 1188;
			else
				if (Occupied)
					if (Part == Part::Head) return 1189;
					else return 1190;
				else
					if (Part == Part::Head) return 1191;
					else return 1192;
		}
		BlockState LightGrayBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace LightGrayCarpet
	{
		constexpr BlockState LightGrayCarpet()
		{
			return 7874;
		}
	}
	namespace LightGrayConcrete
	{
		constexpr BlockState LightGrayConcrete()
		{
			return 9446;
		}
	}
	namespace LightGrayConcretePowder
	{
		constexpr BlockState LightGrayConcretePowder()
		{
			return 9462;
		}
	}
	namespace LightGrayGlazedTerracotta
	{
		constexpr BlockState LightGrayGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9406;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9407;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9408;
			else return 9409;
		}
		BlockState LightGrayGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace LightGrayShulkerBox
	{
		constexpr BlockState LightGrayShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9326;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9327;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9328;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9329;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9330;
			else return 9331;
		}
		BlockState LightGrayShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace LightGrayStainedGlass
	{
		constexpr BlockState LightGrayStainedGlass()
		{
			return 4103;
		}
	}
	namespace LightGrayStainedGlassPane
	{
		constexpr BlockState LightGrayStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7121;
						else return 7122;
					else
						if (West) return 7125;
						else return 7126;
				else
					if (South)
						if (West) return 7129;
						else return 7130;
					else
						if (West) return 7133;
						else return 7134;
			else
				if (North)
					if (South)
						if (West) return 7137;
						else return 7138;
					else
						if (West) return 7141;
						else return 7142;
				else
					if (South)
						if (West) return 7145;
						else return 7146;
					else
						if (West) return 7149;
						else return 7150;
		}
		BlockState LightGrayStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace LightGrayTerracotta
	{
		constexpr BlockState LightGrayTerracotta()
		{
			return 6855;
		}
	}
	namespace LightGrayWallBanner
	{
		constexpr BlockState LightGrayWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8185;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8186;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8187;
			else return 8188;
		}
		BlockState LightGrayWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace LightGrayWool
	{
		constexpr BlockState LightGrayWool()
		{
			return 1392;
		}
	}
	namespace LightWeightedPressurePlate
	{
		constexpr BlockState LightWeightedPressurePlate(const unsigned char Power)
		{
			if (Power == 0) return 6646;
			else if (Power == 1) return 6647;
			else if (Power == 2) return 6648;
			else if (Power == 3) return 6649;
			else if (Power == 4) return 6650;
			else if (Power == 5) return 6651;
			else if (Power == 6) return 6652;
			else if (Power == 7) return 6653;
			else if (Power == 8) return 6654;
			else if (Power == 9) return 6655;
			else if (Power == 10) return 6656;
			else if (Power == 11) return 6657;
			else if (Power == 12) return 6658;
			else if (Power == 13) return 6659;
			else if (Power == 14) return 6660;
			else return 6661;
		}
		BlockState LightWeightedPressurePlate();
		unsigned char Power(BlockState Block);
	}
	namespace Lilac
	{
		enum class Half
		{
			Upper,
			Lower
		};
		constexpr BlockState Lilac(const enum Half Half)
		{
			if (Half == Half::Upper) return 7887;
			else return 7888;
		}
		BlockState Lilac();
		enum Half Half(BlockState Block);
	}
	namespace LilyOfTheValley
	{
		constexpr BlockState LilyOfTheValley()
		{
			return 1424;
		}
	}
	namespace LilyPad
	{
		constexpr BlockState LilyPad()
		{
			return 5014;
		}
	}
	namespace LimeBanner
	{
		constexpr BlockState LimeBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 7977;
			else if (Rotation == 1) return 7978;
			else if (Rotation == 2) return 7979;
			else if (Rotation == 3) return 7980;
			else if (Rotation == 4) return 7981;
			else if (Rotation == 5) return 7982;
			else if (Rotation == 6) return 7983;
			else if (Rotation == 7) return 7984;
			else if (Rotation == 8) return 7985;
			else if (Rotation == 9) return 7986;
			else if (Rotation == 10) return 7987;
			else if (Rotation == 11) return 7988;
			else if (Rotation == 12) return 7989;
			else if (Rotation == 13) return 7990;
			else if (Rotation == 14) return 7991;
			else return 7992;
		}
		BlockState LimeBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace LimeBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState LimeBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1129;
					else return 1130;
				else
					if (Part == Part::Head) return 1131;
					else return 1132;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1133;
					else return 1134;
				else
					if (Part == Part::Head) return 1135;
					else return 1136;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1137;
					else return 1138;
				else
					if (Part == Part::Head) return 1139;
					else return 1140;
			else
				if (Occupied)
					if (Part == Part::Head) return 1141;
					else return 1142;
				else
					if (Part == Part::Head) return 1143;
					else return 1144;
		}
		BlockState LimeBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace LimeCarpet
	{
		constexpr BlockState LimeCarpet()
		{
			return 7871;
		}
	}
	namespace LimeConcrete
	{
		constexpr BlockState LimeConcrete()
		{
			return 9443;
		}
	}
	namespace LimeConcretePowder
	{
		constexpr BlockState LimeConcretePowder()
		{
			return 9459;
		}
	}
	namespace LimeGlazedTerracotta
	{
		constexpr BlockState LimeGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9394;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9395;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9396;
			else return 9397;
		}
		BlockState LimeGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace LimeShulkerBox
	{
		constexpr BlockState LimeShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9308;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9309;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9310;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9311;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9312;
			else return 9313;
		}
		BlockState LimeShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace LimeStainedGlass
	{
		constexpr BlockState LimeStainedGlass()
		{
			return 4100;
		}
	}
	namespace LimeStainedGlassPane
	{
		constexpr BlockState LimeStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7025;
						else return 7026;
					else
						if (West) return 7029;
						else return 7030;
				else
					if (South)
						if (West) return 7033;
						else return 7034;
					else
						if (West) return 7037;
						else return 7038;
			else
				if (North)
					if (South)
						if (West) return 7041;
						else return 7042;
					else
						if (West) return 7045;
						else return 7046;
				else
					if (South)
						if (West) return 7049;
						else return 7050;
					else
						if (West) return 7053;
						else return 7054;
		}
		BlockState LimeStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace LimeTerracotta
	{
		constexpr BlockState LimeTerracotta()
		{
			return 6852;
		}
	}
	namespace LimeWallBanner
	{
		constexpr BlockState LimeWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8173;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8174;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8175;
			else return 8176;
		}
		BlockState LimeWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace LimeWool
	{
		constexpr BlockState LimeWool()
		{
			return 1389;
		}
	}
	namespace Lodestone
	{
		constexpr BlockState Lodestone()
		{
			return 15838;
		}
	}
	namespace Loom
	{
		constexpr BlockState Loom(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 14787;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 14788;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 14789;
			else return 14790;
		}
		BlockState Loom();
		eBlockFace Facing(BlockState Block);
	}
	namespace MagentaBanner
	{
		constexpr BlockState MagentaBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 7929;
			else if (Rotation == 1) return 7930;
			else if (Rotation == 2) return 7931;
			else if (Rotation == 3) return 7932;
			else if (Rotation == 4) return 7933;
			else if (Rotation == 5) return 7934;
			else if (Rotation == 6) return 7935;
			else if (Rotation == 7) return 7936;
			else if (Rotation == 8) return 7937;
			else if (Rotation == 9) return 7938;
			else if (Rotation == 10) return 7939;
			else if (Rotation == 11) return 7940;
			else if (Rotation == 12) return 7941;
			else if (Rotation == 13) return 7942;
			else if (Rotation == 14) return 7943;
			else return 7944;
		}
		BlockState MagentaBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace MagentaBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState MagentaBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1081;
					else return 1082;
				else
					if (Part == Part::Head) return 1083;
					else return 1084;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1085;
					else return 1086;
				else
					if (Part == Part::Head) return 1087;
					else return 1088;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1089;
					else return 1090;
				else
					if (Part == Part::Head) return 1091;
					else return 1092;
			else
				if (Occupied)
					if (Part == Part::Head) return 1093;
					else return 1094;
				else
					if (Part == Part::Head) return 1095;
					else return 1096;
		}
		BlockState MagentaBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace MagentaCarpet
	{
		constexpr BlockState MagentaCarpet()
		{
			return 7868;
		}
	}
	namespace MagentaConcrete
	{
		constexpr BlockState MagentaConcrete()
		{
			return 9440;
		}
	}
	namespace MagentaConcretePowder
	{
		constexpr BlockState MagentaConcretePowder()
		{
			return 9456;
		}
	}
	namespace MagentaGlazedTerracotta
	{
		constexpr BlockState MagentaGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9382;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9383;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9384;
			else return 9385;
		}
		BlockState MagentaGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace MagentaShulkerBox
	{
		constexpr BlockState MagentaShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9290;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9291;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9292;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9293;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9294;
			else return 9295;
		}
		BlockState MagentaShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace MagentaStainedGlass
	{
		constexpr BlockState MagentaStainedGlass()
		{
			return 4097;
		}
	}
	namespace MagentaStainedGlassPane
	{
		constexpr BlockState MagentaStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 6929;
						else return 6930;
					else
						if (West) return 6933;
						else return 6934;
				else
					if (South)
						if (West) return 6937;
						else return 6938;
					else
						if (West) return 6941;
						else return 6942;
			else
				if (North)
					if (South)
						if (West) return 6945;
						else return 6946;
					else
						if (West) return 6949;
						else return 6950;
				else
					if (South)
						if (West) return 6953;
						else return 6954;
					else
						if (West) return 6957;
						else return 6958;
		}
		BlockState MagentaStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace MagentaTerracotta
	{
		constexpr BlockState MagentaTerracotta()
		{
			return 6849;
		}
	}
	namespace MagentaWallBanner
	{
		constexpr BlockState MagentaWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8161;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8162;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8163;
			else return 8164;
		}
		BlockState MagentaWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace MagentaWool
	{
		constexpr BlockState MagentaWool()
		{
			return 1386;
		}
	}
	namespace MagmaBlock
	{
		constexpr BlockState MagmaBlock()
		{
			return 9253;
		}
	}
	namespace Melon
	{
		constexpr BlockState Melon()
		{
			return 4763;
		}
	}
	namespace MelonStem
	{
		constexpr BlockState MelonStem(const unsigned char Age)
		{
			if (Age == 0) return 4780;
			else if (Age == 1) return 4781;
			else if (Age == 2) return 4782;
			else if (Age == 3) return 4783;
			else if (Age == 4) return 4784;
			else if (Age == 5) return 4785;
			else if (Age == 6) return 4786;
			else return 4787;
		}
		BlockState MelonStem();
		unsigned char Age(BlockState Block);
	}
	namespace MossyCobblestone
	{
		constexpr BlockState MossyCobblestone()
		{
			return 1433;
		}
	}
	namespace MossyCobblestoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState MossyCobblestoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10814;
			else if (Type == Type::Bottom) return 10816;
			else return 10818;
		}
		BlockState MossyCobblestoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace MossyCobblestoneStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState MossyCobblestoneStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9990;
					else if (Shape == Shape::InnerLeft) return 9992;
					else if (Shape == Shape::InnerRight) return 9994;
					else if (Shape == Shape::OuterLeft) return 9996;
					else return 9998;
				else
					if (Shape == Shape::Straight) return 10000;
					else if (Shape == Shape::InnerLeft) return 10002;
					else if (Shape == Shape::InnerRight) return 10004;
					else if (Shape == Shape::OuterLeft) return 10006;
					else return 10008;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10010;
					else if (Shape == Shape::InnerLeft) return 10012;
					else if (Shape == Shape::InnerRight) return 10014;
					else if (Shape == Shape::OuterLeft) return 10016;
					else return 10018;
				else
					if (Shape == Shape::Straight) return 10020;
					else if (Shape == Shape::InnerLeft) return 10022;
					else if (Shape == Shape::InnerRight) return 10024;
					else if (Shape == Shape::OuterLeft) return 10026;
					else return 10028;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10030;
					else if (Shape == Shape::InnerLeft) return 10032;
					else if (Shape == Shape::InnerRight) return 10034;
					else if (Shape == Shape::OuterLeft) return 10036;
					else return 10038;
				else
					if (Shape == Shape::Straight) return 10040;
					else if (Shape == Shape::InnerLeft) return 10042;
					else if (Shape == Shape::InnerRight) return 10044;
					else if (Shape == Shape::OuterLeft) return 10046;
					else return 10048;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10050;
					else if (Shape == Shape::InnerLeft) return 10052;
					else if (Shape == Shape::InnerRight) return 10054;
					else if (Shape == Shape::OuterLeft) return 10056;
					else return 10058;
				else
					if (Shape == Shape::Straight) return 10060;
					else if (Shape == Shape::InnerLeft) return 10062;
					else if (Shape == Shape::InnerRight) return 10064;
					else if (Shape == Shape::OuterLeft) return 10066;
					else return 10068;
		}
		BlockState MossyCobblestoneStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace MossyCobblestoneWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState MossyCobblestoneWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 5984;
							else if (West == West::Low) return 5985;
							else return 5986;
						else
							if (West == West::None) return 5990;
							else if (West == West::Low) return 5991;
							else return 5992;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 5996;
							else if (West == West::Low) return 5997;
							else return 5998;
						else
							if (West == West::None) return 6002;
							else if (West == West::Low) return 6003;
							else return 6004;
					else
						if (Up)
							if (West == West::None) return 6008;
							else if (West == West::Low) return 6009;
							else return 6010;
						else
							if (West == West::None) return 6014;
							else if (West == West::Low) return 6015;
							else return 6016;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 6020;
							else if (West == West::Low) return 6021;
							else return 6022;
						else
							if (West == West::None) return 6026;
							else if (West == West::Low) return 6027;
							else return 6028;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 6032;
							else if (West == West::Low) return 6033;
							else return 6034;
						else
							if (West == West::None) return 6038;
							else if (West == West::Low) return 6039;
							else return 6040;
					else
						if (Up)
							if (West == West::None) return 6044;
							else if (West == West::Low) return 6045;
							else return 6046;
						else
							if (West == West::None) return 6050;
							else if (West == West::Low) return 6051;
							else return 6052;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 6056;
							else if (West == West::Low) return 6057;
							else return 6058;
						else
							if (West == West::None) return 6062;
							else if (West == West::Low) return 6063;
							else return 6064;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 6068;
							else if (West == West::Low) return 6069;
							else return 6070;
						else
							if (West == West::None) return 6074;
							else if (West == West::Low) return 6075;
							else return 6076;
					else
						if (Up)
							if (West == West::None) return 6080;
							else if (West == West::Low) return 6081;
							else return 6082;
						else
							if (West == West::None) return 6086;
							else if (West == West::Low) return 6087;
							else return 6088;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 6092;
							else if (West == West::Low) return 6093;
							else return 6094;
						else
							if (West == West::None) return 6098;
							else if (West == West::Low) return 6099;
							else return 6100;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 6104;
							else if (West == West::Low) return 6105;
							else return 6106;
						else
							if (West == West::None) return 6110;
							else if (West == West::Low) return 6111;
							else return 6112;
					else
						if (Up)
							if (West == West::None) return 6116;
							else if (West == West::Low) return 6117;
							else return 6118;
						else
							if (West == West::None) return 6122;
							else if (West == West::Low) return 6123;
							else return 6124;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 6128;
							else if (West == West::Low) return 6129;
							else return 6130;
						else
							if (West == West::None) return 6134;
							else if (West == West::Low) return 6135;
							else return 6136;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 6140;
							else if (West == West::Low) return 6141;
							else return 6142;
						else
							if (West == West::None) return 6146;
							else if (West == West::Low) return 6147;
							else return 6148;
					else
						if (Up)
							if (West == West::None) return 6152;
							else if (West == West::Low) return 6153;
							else return 6154;
						else
							if (West == West::None) return 6158;
							else if (West == West::Low) return 6159;
							else return 6160;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 6164;
							else if (West == West::Low) return 6165;
							else return 6166;
						else
							if (West == West::None) return 6170;
							else if (West == West::Low) return 6171;
							else return 6172;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 6176;
							else if (West == West::Low) return 6177;
							else return 6178;
						else
							if (West == West::None) return 6182;
							else if (West == West::Low) return 6183;
							else return 6184;
					else
						if (Up)
							if (West == West::None) return 6188;
							else if (West == West::Low) return 6189;
							else return 6190;
						else
							if (West == West::None) return 6194;
							else if (West == West::Low) return 6195;
							else return 6196;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 6200;
							else if (West == West::Low) return 6201;
							else return 6202;
						else
							if (West == West::None) return 6206;
							else if (West == West::Low) return 6207;
							else return 6208;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 6212;
							else if (West == West::Low) return 6213;
							else return 6214;
						else
							if (West == West::None) return 6218;
							else if (West == West::Low) return 6219;
							else return 6220;
					else
						if (Up)
							if (West == West::None) return 6224;
							else if (West == West::Low) return 6225;
							else return 6226;
						else
							if (West == West::None) return 6230;
							else if (West == West::Low) return 6231;
							else return 6232;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 6236;
							else if (West == West::Low) return 6237;
							else return 6238;
						else
							if (West == West::None) return 6242;
							else if (West == West::Low) return 6243;
							else return 6244;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 6248;
							else if (West == West::Low) return 6249;
							else return 6250;
						else
							if (West == West::None) return 6254;
							else if (West == West::Low) return 6255;
							else return 6256;
					else
						if (Up)
							if (West == West::None) return 6260;
							else if (West == West::Low) return 6261;
							else return 6262;
						else
							if (West == West::None) return 6266;
							else if (West == West::Low) return 6267;
							else return 6268;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 6272;
							else if (West == West::Low) return 6273;
							else return 6274;
						else
							if (West == West::None) return 6278;
							else if (West == West::Low) return 6279;
							else return 6280;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 6284;
							else if (West == West::Low) return 6285;
							else return 6286;
						else
							if (West == West::None) return 6290;
							else if (West == West::Low) return 6291;
							else return 6292;
					else
						if (Up)
							if (West == West::None) return 6296;
							else if (West == West::Low) return 6297;
							else return 6298;
						else
							if (West == West::None) return 6302;
							else if (West == West::Low) return 6303;
							else return 6304;
		}
		BlockState MossyCobblestoneWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace MossyStoneBrickSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState MossyStoneBrickSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10802;
			else if (Type == Type::Bottom) return 10804;
			else return 10806;
		}
		BlockState MossyStoneBrickSlab();
		enum Type Type(BlockState Block);
	}
	namespace MossyStoneBrickStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState MossyStoneBrickStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9830;
					else if (Shape == Shape::InnerLeft) return 9832;
					else if (Shape == Shape::InnerRight) return 9834;
					else if (Shape == Shape::OuterLeft) return 9836;
					else return 9838;
				else
					if (Shape == Shape::Straight) return 9840;
					else if (Shape == Shape::InnerLeft) return 9842;
					else if (Shape == Shape::InnerRight) return 9844;
					else if (Shape == Shape::OuterLeft) return 9846;
					else return 9848;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9850;
					else if (Shape == Shape::InnerLeft) return 9852;
					else if (Shape == Shape::InnerRight) return 9854;
					else if (Shape == Shape::OuterLeft) return 9856;
					else return 9858;
				else
					if (Shape == Shape::Straight) return 9860;
					else if (Shape == Shape::InnerLeft) return 9862;
					else if (Shape == Shape::InnerRight) return 9864;
					else if (Shape == Shape::OuterLeft) return 9866;
					else return 9868;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9870;
					else if (Shape == Shape::InnerLeft) return 9872;
					else if (Shape == Shape::InnerRight) return 9874;
					else if (Shape == Shape::OuterLeft) return 9876;
					else return 9878;
				else
					if (Shape == Shape::Straight) return 9880;
					else if (Shape == Shape::InnerLeft) return 9882;
					else if (Shape == Shape::InnerRight) return 9884;
					else if (Shape == Shape::OuterLeft) return 9886;
					else return 9888;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9890;
					else if (Shape == Shape::InnerLeft) return 9892;
					else if (Shape == Shape::InnerRight) return 9894;
					else if (Shape == Shape::OuterLeft) return 9896;
					else return 9898;
				else
					if (Shape == Shape::Straight) return 9900;
					else if (Shape == Shape::InnerLeft) return 9902;
					else if (Shape == Shape::InnerRight) return 9904;
					else if (Shape == Shape::OuterLeft) return 9906;
					else return 9908;
		}
		BlockState MossyStoneBrickStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace MossyStoneBrickWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState MossyStoneBrickWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11842;
							else if (West == West::Low) return 11843;
							else return 11844;
						else
							if (West == West::None) return 11848;
							else if (West == West::Low) return 11849;
							else return 11850;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11854;
							else if (West == West::Low) return 11855;
							else return 11856;
						else
							if (West == West::None) return 11860;
							else if (West == West::Low) return 11861;
							else return 11862;
					else
						if (Up)
							if (West == West::None) return 11866;
							else if (West == West::Low) return 11867;
							else return 11868;
						else
							if (West == West::None) return 11872;
							else if (West == West::Low) return 11873;
							else return 11874;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11878;
							else if (West == West::Low) return 11879;
							else return 11880;
						else
							if (West == West::None) return 11884;
							else if (West == West::Low) return 11885;
							else return 11886;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11890;
							else if (West == West::Low) return 11891;
							else return 11892;
						else
							if (West == West::None) return 11896;
							else if (West == West::Low) return 11897;
							else return 11898;
					else
						if (Up)
							if (West == West::None) return 11902;
							else if (West == West::Low) return 11903;
							else return 11904;
						else
							if (West == West::None) return 11908;
							else if (West == West::Low) return 11909;
							else return 11910;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11914;
							else if (West == West::Low) return 11915;
							else return 11916;
						else
							if (West == West::None) return 11920;
							else if (West == West::Low) return 11921;
							else return 11922;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11926;
							else if (West == West::Low) return 11927;
							else return 11928;
						else
							if (West == West::None) return 11932;
							else if (West == West::Low) return 11933;
							else return 11934;
					else
						if (Up)
							if (West == West::None) return 11938;
							else if (West == West::Low) return 11939;
							else return 11940;
						else
							if (West == West::None) return 11944;
							else if (West == West::Low) return 11945;
							else return 11946;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11950;
							else if (West == West::Low) return 11951;
							else return 11952;
						else
							if (West == West::None) return 11956;
							else if (West == West::Low) return 11957;
							else return 11958;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11962;
							else if (West == West::Low) return 11963;
							else return 11964;
						else
							if (West == West::None) return 11968;
							else if (West == West::Low) return 11969;
							else return 11970;
					else
						if (Up)
							if (West == West::None) return 11974;
							else if (West == West::Low) return 11975;
							else return 11976;
						else
							if (West == West::None) return 11980;
							else if (West == West::Low) return 11981;
							else return 11982;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11986;
							else if (West == West::Low) return 11987;
							else return 11988;
						else
							if (West == West::None) return 11992;
							else if (West == West::Low) return 11993;
							else return 11994;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11998;
							else if (West == West::Low) return 11999;
							else return 12000;
						else
							if (West == West::None) return 12004;
							else if (West == West::Low) return 12005;
							else return 12006;
					else
						if (Up)
							if (West == West::None) return 12010;
							else if (West == West::Low) return 12011;
							else return 12012;
						else
							if (West == West::None) return 12016;
							else if (West == West::Low) return 12017;
							else return 12018;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12022;
							else if (West == West::Low) return 12023;
							else return 12024;
						else
							if (West == West::None) return 12028;
							else if (West == West::Low) return 12029;
							else return 12030;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12034;
							else if (West == West::Low) return 12035;
							else return 12036;
						else
							if (West == West::None) return 12040;
							else if (West == West::Low) return 12041;
							else return 12042;
					else
						if (Up)
							if (West == West::None) return 12046;
							else if (West == West::Low) return 12047;
							else return 12048;
						else
							if (West == West::None) return 12052;
							else if (West == West::Low) return 12053;
							else return 12054;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12058;
							else if (West == West::Low) return 12059;
							else return 12060;
						else
							if (West == West::None) return 12064;
							else if (West == West::Low) return 12065;
							else return 12066;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12070;
							else if (West == West::Low) return 12071;
							else return 12072;
						else
							if (West == West::None) return 12076;
							else if (West == West::Low) return 12077;
							else return 12078;
					else
						if (Up)
							if (West == West::None) return 12082;
							else if (West == West::Low) return 12083;
							else return 12084;
						else
							if (West == West::None) return 12088;
							else if (West == West::Low) return 12089;
							else return 12090;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12094;
							else if (West == West::Low) return 12095;
							else return 12096;
						else
							if (West == West::None) return 12100;
							else if (West == West::Low) return 12101;
							else return 12102;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12106;
							else if (West == West::Low) return 12107;
							else return 12108;
						else
							if (West == West::None) return 12112;
							else if (West == West::Low) return 12113;
							else return 12114;
					else
						if (Up)
							if (West == West::None) return 12118;
							else if (West == West::Low) return 12119;
							else return 12120;
						else
							if (West == West::None) return 12124;
							else if (West == West::Low) return 12125;
							else return 12126;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12130;
							else if (West == West::Low) return 12131;
							else return 12132;
						else
							if (West == West::None) return 12136;
							else if (West == West::Low) return 12137;
							else return 12138;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12142;
							else if (West == West::Low) return 12143;
							else return 12144;
						else
							if (West == West::None) return 12148;
							else if (West == West::Low) return 12149;
							else return 12150;
					else
						if (Up)
							if (West == West::None) return 12154;
							else if (West == West::Low) return 12155;
							else return 12156;
						else
							if (West == West::None) return 12160;
							else if (West == West::Low) return 12161;
							else return 12162;
		}
		BlockState MossyStoneBrickWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace MossyStoneBricks
	{
		constexpr BlockState MossyStoneBricks()
		{
			return 4496;
		}
	}
	namespace MovingPiston
	{
		enum class Type
		{
			Normal,
			Sticky
		};
		constexpr BlockState MovingPiston(const eBlockFace Facing, const enum Type Type)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Type == Type::Normal) return 1400;
				else return 1401;
			else if (Facing == eBlockFace::BLOCK_FACE_XP)
				if (Type == Type::Normal) return 1402;
				else return 1403;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Type == Type::Normal) return 1404;
				else return 1405;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Type == Type::Normal) return 1406;
				else return 1407;
			else if (Facing == eBlockFace::BLOCK_FACE_YP)
				if (Type == Type::Normal) return 1408;
				else return 1409;
			else
				if (Type == Type::Normal) return 1410;
				else return 1411;
		}
		BlockState MovingPiston();
		eBlockFace Facing(BlockState Block);
		enum Type Type(BlockState Block);
	}
	namespace MushroomStem
	{
		constexpr BlockState MushroomStem(const bool Down, const bool East, const bool North, const bool South, const bool Up, const bool West)
		{
			if (Down)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 4633;
								else return 4634;
							else
								if (West) return 4635;
								else return 4636;
						else
							if (Up)
								if (West) return 4637;
								else return 4638;
							else
								if (West) return 4639;
								else return 4640;
					else
						if (South)
							if (Up)
								if (West) return 4641;
								else return 4642;
							else
								if (West) return 4643;
								else return 4644;
						else
							if (Up)
								if (West) return 4645;
								else return 4646;
							else
								if (West) return 4647;
								else return 4648;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 4649;
								else return 4650;
							else
								if (West) return 4651;
								else return 4652;
						else
							if (Up)
								if (West) return 4653;
								else return 4654;
							else
								if (West) return 4655;
								else return 4656;
					else
						if (South)
							if (Up)
								if (West) return 4657;
								else return 4658;
							else
								if (West) return 4659;
								else return 4660;
						else
							if (Up)
								if (West) return 4661;
								else return 4662;
							else
								if (West) return 4663;
								else return 4664;
			else
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 4665;
								else return 4666;
							else
								if (West) return 4667;
								else return 4668;
						else
							if (Up)
								if (West) return 4669;
								else return 4670;
							else
								if (West) return 4671;
								else return 4672;
					else
						if (South)
							if (Up)
								if (West) return 4673;
								else return 4674;
							else
								if (West) return 4675;
								else return 4676;
						else
							if (Up)
								if (West) return 4677;
								else return 4678;
							else
								if (West) return 4679;
								else return 4680;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 4681;
								else return 4682;
							else
								if (West) return 4683;
								else return 4684;
						else
							if (Up)
								if (West) return 4685;
								else return 4686;
							else
								if (West) return 4687;
								else return 4688;
					else
						if (South)
							if (Up)
								if (West) return 4689;
								else return 4690;
							else
								if (West) return 4691;
								else return 4692;
						else
							if (Up)
								if (West) return 4693;
								else return 4694;
							else
								if (West) return 4695;
								else return 4696;
		}
		BlockState MushroomStem();
		bool Down(BlockState Block);
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool Up(BlockState Block);
		bool West(BlockState Block);
	}
	namespace Mycelium
	{
		constexpr BlockState Mycelium(const bool Snowy)
		{
			if (Snowy) return 5012;
			else return 5013;
		}
		BlockState Mycelium();
		bool Snowy(BlockState Block);
	}
	namespace NetherBrickFence
	{
		constexpr BlockState NetherBrickFence(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 5018;
						else return 5019;
					else
						if (West) return 5022;
						else return 5023;
				else
					if (South)
						if (West) return 5026;
						else return 5027;
					else
						if (West) return 5030;
						else return 5031;
			else
				if (North)
					if (South)
						if (West) return 5034;
						else return 5035;
					else
						if (West) return 5038;
						else return 5039;
				else
					if (South)
						if (West) return 5042;
						else return 5043;
					else
						if (West) return 5046;
						else return 5047;
		}
		BlockState NetherBrickFence();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace NetherBrickSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState NetherBrickSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8385;
			else if (Type == Type::Bottom) return 8387;
			else return 8389;
		}
		BlockState NetherBrickSlab();
		enum Type Type(BlockState Block);
	}
	namespace NetherBrickStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState NetherBrickStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5049;
					else if (Shape == Shape::InnerLeft) return 5051;
					else if (Shape == Shape::InnerRight) return 5053;
					else if (Shape == Shape::OuterLeft) return 5055;
					else return 5057;
				else
					if (Shape == Shape::Straight) return 5059;
					else if (Shape == Shape::InnerLeft) return 5061;
					else if (Shape == Shape::InnerRight) return 5063;
					else if (Shape == Shape::OuterLeft) return 5065;
					else return 5067;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5069;
					else if (Shape == Shape::InnerLeft) return 5071;
					else if (Shape == Shape::InnerRight) return 5073;
					else if (Shape == Shape::OuterLeft) return 5075;
					else return 5077;
				else
					if (Shape == Shape::Straight) return 5079;
					else if (Shape == Shape::InnerLeft) return 5081;
					else if (Shape == Shape::InnerRight) return 5083;
					else if (Shape == Shape::OuterLeft) return 5085;
					else return 5087;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5089;
					else if (Shape == Shape::InnerLeft) return 5091;
					else if (Shape == Shape::InnerRight) return 5093;
					else if (Shape == Shape::OuterLeft) return 5095;
					else return 5097;
				else
					if (Shape == Shape::Straight) return 5099;
					else if (Shape == Shape::InnerLeft) return 5101;
					else if (Shape == Shape::InnerRight) return 5103;
					else if (Shape == Shape::OuterLeft) return 5105;
					else return 5107;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5109;
					else if (Shape == Shape::InnerLeft) return 5111;
					else if (Shape == Shape::InnerRight) return 5113;
					else if (Shape == Shape::OuterLeft) return 5115;
					else return 5117;
				else
					if (Shape == Shape::Straight) return 5119;
					else if (Shape == Shape::InnerLeft) return 5121;
					else if (Shape == Shape::InnerRight) return 5123;
					else if (Shape == Shape::OuterLeft) return 5125;
					else return 5127;
		}
		BlockState NetherBrickStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace NetherBrickWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState NetherBrickWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12814;
							else if (West == West::Low) return 12815;
							else return 12816;
						else
							if (West == West::None) return 12820;
							else if (West == West::Low) return 12821;
							else return 12822;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12826;
							else if (West == West::Low) return 12827;
							else return 12828;
						else
							if (West == West::None) return 12832;
							else if (West == West::Low) return 12833;
							else return 12834;
					else
						if (Up)
							if (West == West::None) return 12838;
							else if (West == West::Low) return 12839;
							else return 12840;
						else
							if (West == West::None) return 12844;
							else if (West == West::Low) return 12845;
							else return 12846;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12850;
							else if (West == West::Low) return 12851;
							else return 12852;
						else
							if (West == West::None) return 12856;
							else if (West == West::Low) return 12857;
							else return 12858;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12862;
							else if (West == West::Low) return 12863;
							else return 12864;
						else
							if (West == West::None) return 12868;
							else if (West == West::Low) return 12869;
							else return 12870;
					else
						if (Up)
							if (West == West::None) return 12874;
							else if (West == West::Low) return 12875;
							else return 12876;
						else
							if (West == West::None) return 12880;
							else if (West == West::Low) return 12881;
							else return 12882;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12886;
							else if (West == West::Low) return 12887;
							else return 12888;
						else
							if (West == West::None) return 12892;
							else if (West == West::Low) return 12893;
							else return 12894;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12898;
							else if (West == West::Low) return 12899;
							else return 12900;
						else
							if (West == West::None) return 12904;
							else if (West == West::Low) return 12905;
							else return 12906;
					else
						if (Up)
							if (West == West::None) return 12910;
							else if (West == West::Low) return 12911;
							else return 12912;
						else
							if (West == West::None) return 12916;
							else if (West == West::Low) return 12917;
							else return 12918;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12922;
							else if (West == West::Low) return 12923;
							else return 12924;
						else
							if (West == West::None) return 12928;
							else if (West == West::Low) return 12929;
							else return 12930;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12934;
							else if (West == West::Low) return 12935;
							else return 12936;
						else
							if (West == West::None) return 12940;
							else if (West == West::Low) return 12941;
							else return 12942;
					else
						if (Up)
							if (West == West::None) return 12946;
							else if (West == West::Low) return 12947;
							else return 12948;
						else
							if (West == West::None) return 12952;
							else if (West == West::Low) return 12953;
							else return 12954;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12958;
							else if (West == West::Low) return 12959;
							else return 12960;
						else
							if (West == West::None) return 12964;
							else if (West == West::Low) return 12965;
							else return 12966;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12970;
							else if (West == West::Low) return 12971;
							else return 12972;
						else
							if (West == West::None) return 12976;
							else if (West == West::Low) return 12977;
							else return 12978;
					else
						if (Up)
							if (West == West::None) return 12982;
							else if (West == West::Low) return 12983;
							else return 12984;
						else
							if (West == West::None) return 12988;
							else if (West == West::Low) return 12989;
							else return 12990;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12994;
							else if (West == West::Low) return 12995;
							else return 12996;
						else
							if (West == West::None) return 13000;
							else if (West == West::Low) return 13001;
							else return 13002;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13006;
							else if (West == West::Low) return 13007;
							else return 13008;
						else
							if (West == West::None) return 13012;
							else if (West == West::Low) return 13013;
							else return 13014;
					else
						if (Up)
							if (West == West::None) return 13018;
							else if (West == West::Low) return 13019;
							else return 13020;
						else
							if (West == West::None) return 13024;
							else if (West == West::Low) return 13025;
							else return 13026;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13030;
							else if (West == West::Low) return 13031;
							else return 13032;
						else
							if (West == West::None) return 13036;
							else if (West == West::Low) return 13037;
							else return 13038;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13042;
							else if (West == West::Low) return 13043;
							else return 13044;
						else
							if (West == West::None) return 13048;
							else if (West == West::Low) return 13049;
							else return 13050;
					else
						if (Up)
							if (West == West::None) return 13054;
							else if (West == West::Low) return 13055;
							else return 13056;
						else
							if (West == West::None) return 13060;
							else if (West == West::Low) return 13061;
							else return 13062;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13066;
							else if (West == West::Low) return 13067;
							else return 13068;
						else
							if (West == West::None) return 13072;
							else if (West == West::Low) return 13073;
							else return 13074;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13078;
							else if (West == West::Low) return 13079;
							else return 13080;
						else
							if (West == West::None) return 13084;
							else if (West == West::Low) return 13085;
							else return 13086;
					else
						if (Up)
							if (West == West::None) return 13090;
							else if (West == West::Low) return 13091;
							else return 13092;
						else
							if (West == West::None) return 13096;
							else if (West == West::Low) return 13097;
							else return 13098;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13102;
							else if (West == West::Low) return 13103;
							else return 13104;
						else
							if (West == West::None) return 13108;
							else if (West == West::Low) return 13109;
							else return 13110;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13114;
							else if (West == West::Low) return 13115;
							else return 13116;
						else
							if (West == West::None) return 13120;
							else if (West == West::Low) return 13121;
							else return 13122;
					else
						if (Up)
							if (West == West::None) return 13126;
							else if (West == West::Low) return 13127;
							else return 13128;
						else
							if (West == West::None) return 13132;
							else if (West == West::Low) return 13133;
							else return 13134;
		}
		BlockState NetherBrickWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace NetherBricks
	{
		constexpr BlockState NetherBricks()
		{
			return 5015;
		}
	}
	namespace NetherGoldOre
	{
		constexpr BlockState NetherGoldOre()
		{
			return 72;
		}
	}
	namespace NetherPortal
	{
		enum class Axis
		{
			X,
			Z
		};
		constexpr BlockState NetherPortal(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 4014;
			else return 4015;
		}
		BlockState NetherPortal();
		enum Axis Axis(BlockState Block);
	}
	namespace NetherQuartzOre
	{
		constexpr BlockState NetherQuartzOre()
		{
			return 6727;
		}
	}
	namespace NetherSprouts
	{
		constexpr BlockState NetherSprouts()
		{
			return 14974;
		}
	}
	namespace NetherWart
	{
		constexpr BlockState NetherWart(const unsigned char Age)
		{
			if (Age == 0) return 5128;
			else if (Age == 1) return 5129;
			else if (Age == 2) return 5130;
			else return 5131;
		}
		BlockState NetherWart();
		unsigned char Age(BlockState Block);
	}
	namespace NetherWartBlock
	{
		constexpr BlockState NetherWartBlock()
		{
			return 9254;
		}
	}
	namespace NetheriteBlock
	{
		constexpr BlockState NetheriteBlock()
		{
			return 15826;
		}
	}
	namespace Netherrack
	{
		constexpr BlockState Netherrack()
		{
			return 3999;
		}
	}
	namespace NoteBlock
	{
		enum class Instrument
		{
			Harp,
			Basedrum,
			Snare,
			Hat,
			Bass,
			Flute,
			Bell,
			Guitar,
			Chime,
			Xylophone,
			IronXylophone,
			CowBell,
			Didgeridoo,
			Bit,
			Banjo,
			Pling
		};
		constexpr BlockState NoteBlock(const enum Instrument Instrument, const unsigned char Note, const bool Powered)
		{
			if (Instrument == Instrument::Harp)
				if (Note == 0)
					if (Powered) return 249;
					else return 250;
				else if (Note == 1)
					if (Powered) return 251;
					else return 252;
				else if (Note == 2)
					if (Powered) return 253;
					else return 254;
				else if (Note == 3)
					if (Powered) return 255;
					else return 256;
				else if (Note == 4)
					if (Powered) return 257;
					else return 258;
				else if (Note == 5)
					if (Powered) return 259;
					else return 260;
				else if (Note == 6)
					if (Powered) return 261;
					else return 262;
				else if (Note == 7)
					if (Powered) return 263;
					else return 264;
				else if (Note == 8)
					if (Powered) return 265;
					else return 266;
				else if (Note == 9)
					if (Powered) return 267;
					else return 268;
				else if (Note == 10)
					if (Powered) return 269;
					else return 270;
				else if (Note == 11)
					if (Powered) return 271;
					else return 272;
				else if (Note == 12)
					if (Powered) return 273;
					else return 274;
				else if (Note == 13)
					if (Powered) return 275;
					else return 276;
				else if (Note == 14)
					if (Powered) return 277;
					else return 278;
				else if (Note == 15)
					if (Powered) return 279;
					else return 280;
				else if (Note == 16)
					if (Powered) return 281;
					else return 282;
				else if (Note == 17)
					if (Powered) return 283;
					else return 284;
				else if (Note == 18)
					if (Powered) return 285;
					else return 286;
				else if (Note == 19)
					if (Powered) return 287;
					else return 288;
				else if (Note == 20)
					if (Powered) return 289;
					else return 290;
				else if (Note == 21)
					if (Powered) return 291;
					else return 292;
				else if (Note == 22)
					if (Powered) return 293;
					else return 294;
				else if (Note == 23)
					if (Powered) return 295;
					else return 296;
				else
					if (Powered) return 297;
					else return 298;
			else if (Instrument == Instrument::Basedrum)
				if (Note == 0)
					if (Powered) return 299;
					else return 300;
				else if (Note == 1)
					if (Powered) return 301;
					else return 302;
				else if (Note == 2)
					if (Powered) return 303;
					else return 304;
				else if (Note == 3)
					if (Powered) return 305;
					else return 306;
				else if (Note == 4)
					if (Powered) return 307;
					else return 308;
				else if (Note == 5)
					if (Powered) return 309;
					else return 310;
				else if (Note == 6)
					if (Powered) return 311;
					else return 312;
				else if (Note == 7)
					if (Powered) return 313;
					else return 314;
				else if (Note == 8)
					if (Powered) return 315;
					else return 316;
				else if (Note == 9)
					if (Powered) return 317;
					else return 318;
				else if (Note == 10)
					if (Powered) return 319;
					else return 320;
				else if (Note == 11)
					if (Powered) return 321;
					else return 322;
				else if (Note == 12)
					if (Powered) return 323;
					else return 324;
				else if (Note == 13)
					if (Powered) return 325;
					else return 326;
				else if (Note == 14)
					if (Powered) return 327;
					else return 328;
				else if (Note == 15)
					if (Powered) return 329;
					else return 330;
				else if (Note == 16)
					if (Powered) return 331;
					else return 332;
				else if (Note == 17)
					if (Powered) return 333;
					else return 334;
				else if (Note == 18)
					if (Powered) return 335;
					else return 336;
				else if (Note == 19)
					if (Powered) return 337;
					else return 338;
				else if (Note == 20)
					if (Powered) return 339;
					else return 340;
				else if (Note == 21)
					if (Powered) return 341;
					else return 342;
				else if (Note == 22)
					if (Powered) return 343;
					else return 344;
				else if (Note == 23)
					if (Powered) return 345;
					else return 346;
				else
					if (Powered) return 347;
					else return 348;
			else if (Instrument == Instrument::Snare)
				if (Note == 0)
					if (Powered) return 349;
					else return 350;
				else if (Note == 1)
					if (Powered) return 351;
					else return 352;
				else if (Note == 2)
					if (Powered) return 353;
					else return 354;
				else if (Note == 3)
					if (Powered) return 355;
					else return 356;
				else if (Note == 4)
					if (Powered) return 357;
					else return 358;
				else if (Note == 5)
					if (Powered) return 359;
					else return 360;
				else if (Note == 6)
					if (Powered) return 361;
					else return 362;
				else if (Note == 7)
					if (Powered) return 363;
					else return 364;
				else if (Note == 8)
					if (Powered) return 365;
					else return 366;
				else if (Note == 9)
					if (Powered) return 367;
					else return 368;
				else if (Note == 10)
					if (Powered) return 369;
					else return 370;
				else if (Note == 11)
					if (Powered) return 371;
					else return 372;
				else if (Note == 12)
					if (Powered) return 373;
					else return 374;
				else if (Note == 13)
					if (Powered) return 375;
					else return 376;
				else if (Note == 14)
					if (Powered) return 377;
					else return 378;
				else if (Note == 15)
					if (Powered) return 379;
					else return 380;
				else if (Note == 16)
					if (Powered) return 381;
					else return 382;
				else if (Note == 17)
					if (Powered) return 383;
					else return 384;
				else if (Note == 18)
					if (Powered) return 385;
					else return 386;
				else if (Note == 19)
					if (Powered) return 387;
					else return 388;
				else if (Note == 20)
					if (Powered) return 389;
					else return 390;
				else if (Note == 21)
					if (Powered) return 391;
					else return 392;
				else if (Note == 22)
					if (Powered) return 393;
					else return 394;
				else if (Note == 23)
					if (Powered) return 395;
					else return 396;
				else
					if (Powered) return 397;
					else return 398;
			else if (Instrument == Instrument::Hat)
				if (Note == 0)
					if (Powered) return 399;
					else return 400;
				else if (Note == 1)
					if (Powered) return 401;
					else return 402;
				else if (Note == 2)
					if (Powered) return 403;
					else return 404;
				else if (Note == 3)
					if (Powered) return 405;
					else return 406;
				else if (Note == 4)
					if (Powered) return 407;
					else return 408;
				else if (Note == 5)
					if (Powered) return 409;
					else return 410;
				else if (Note == 6)
					if (Powered) return 411;
					else return 412;
				else if (Note == 7)
					if (Powered) return 413;
					else return 414;
				else if (Note == 8)
					if (Powered) return 415;
					else return 416;
				else if (Note == 9)
					if (Powered) return 417;
					else return 418;
				else if (Note == 10)
					if (Powered) return 419;
					else return 420;
				else if (Note == 11)
					if (Powered) return 421;
					else return 422;
				else if (Note == 12)
					if (Powered) return 423;
					else return 424;
				else if (Note == 13)
					if (Powered) return 425;
					else return 426;
				else if (Note == 14)
					if (Powered) return 427;
					else return 428;
				else if (Note == 15)
					if (Powered) return 429;
					else return 430;
				else if (Note == 16)
					if (Powered) return 431;
					else return 432;
				else if (Note == 17)
					if (Powered) return 433;
					else return 434;
				else if (Note == 18)
					if (Powered) return 435;
					else return 436;
				else if (Note == 19)
					if (Powered) return 437;
					else return 438;
				else if (Note == 20)
					if (Powered) return 439;
					else return 440;
				else if (Note == 21)
					if (Powered) return 441;
					else return 442;
				else if (Note == 22)
					if (Powered) return 443;
					else return 444;
				else if (Note == 23)
					if (Powered) return 445;
					else return 446;
				else
					if (Powered) return 447;
					else return 448;
			else if (Instrument == Instrument::Bass)
				if (Note == 0)
					if (Powered) return 449;
					else return 450;
				else if (Note == 1)
					if (Powered) return 451;
					else return 452;
				else if (Note == 2)
					if (Powered) return 453;
					else return 454;
				else if (Note == 3)
					if (Powered) return 455;
					else return 456;
				else if (Note == 4)
					if (Powered) return 457;
					else return 458;
				else if (Note == 5)
					if (Powered) return 459;
					else return 460;
				else if (Note == 6)
					if (Powered) return 461;
					else return 462;
				else if (Note == 7)
					if (Powered) return 463;
					else return 464;
				else if (Note == 8)
					if (Powered) return 465;
					else return 466;
				else if (Note == 9)
					if (Powered) return 467;
					else return 468;
				else if (Note == 10)
					if (Powered) return 469;
					else return 470;
				else if (Note == 11)
					if (Powered) return 471;
					else return 472;
				else if (Note == 12)
					if (Powered) return 473;
					else return 474;
				else if (Note == 13)
					if (Powered) return 475;
					else return 476;
				else if (Note == 14)
					if (Powered) return 477;
					else return 478;
				else if (Note == 15)
					if (Powered) return 479;
					else return 480;
				else if (Note == 16)
					if (Powered) return 481;
					else return 482;
				else if (Note == 17)
					if (Powered) return 483;
					else return 484;
				else if (Note == 18)
					if (Powered) return 485;
					else return 486;
				else if (Note == 19)
					if (Powered) return 487;
					else return 488;
				else if (Note == 20)
					if (Powered) return 489;
					else return 490;
				else if (Note == 21)
					if (Powered) return 491;
					else return 492;
				else if (Note == 22)
					if (Powered) return 493;
					else return 494;
				else if (Note == 23)
					if (Powered) return 495;
					else return 496;
				else
					if (Powered) return 497;
					else return 498;
			else if (Instrument == Instrument::Flute)
				if (Note == 0)
					if (Powered) return 499;
					else return 500;
				else if (Note == 1)
					if (Powered) return 501;
					else return 502;
				else if (Note == 2)
					if (Powered) return 503;
					else return 504;
				else if (Note == 3)
					if (Powered) return 505;
					else return 506;
				else if (Note == 4)
					if (Powered) return 507;
					else return 508;
				else if (Note == 5)
					if (Powered) return 509;
					else return 510;
				else if (Note == 6)
					if (Powered) return 511;
					else return 512;
				else if (Note == 7)
					if (Powered) return 513;
					else return 514;
				else if (Note == 8)
					if (Powered) return 515;
					else return 516;
				else if (Note == 9)
					if (Powered) return 517;
					else return 518;
				else if (Note == 10)
					if (Powered) return 519;
					else return 520;
				else if (Note == 11)
					if (Powered) return 521;
					else return 522;
				else if (Note == 12)
					if (Powered) return 523;
					else return 524;
				else if (Note == 13)
					if (Powered) return 525;
					else return 526;
				else if (Note == 14)
					if (Powered) return 527;
					else return 528;
				else if (Note == 15)
					if (Powered) return 529;
					else return 530;
				else if (Note == 16)
					if (Powered) return 531;
					else return 532;
				else if (Note == 17)
					if (Powered) return 533;
					else return 534;
				else if (Note == 18)
					if (Powered) return 535;
					else return 536;
				else if (Note == 19)
					if (Powered) return 537;
					else return 538;
				else if (Note == 20)
					if (Powered) return 539;
					else return 540;
				else if (Note == 21)
					if (Powered) return 541;
					else return 542;
				else if (Note == 22)
					if (Powered) return 543;
					else return 544;
				else if (Note == 23)
					if (Powered) return 545;
					else return 546;
				else
					if (Powered) return 547;
					else return 548;
			else if (Instrument == Instrument::Bell)
				if (Note == 0)
					if (Powered) return 549;
					else return 550;
				else if (Note == 1)
					if (Powered) return 551;
					else return 552;
				else if (Note == 2)
					if (Powered) return 553;
					else return 554;
				else if (Note == 3)
					if (Powered) return 555;
					else return 556;
				else if (Note == 4)
					if (Powered) return 557;
					else return 558;
				else if (Note == 5)
					if (Powered) return 559;
					else return 560;
				else if (Note == 6)
					if (Powered) return 561;
					else return 562;
				else if (Note == 7)
					if (Powered) return 563;
					else return 564;
				else if (Note == 8)
					if (Powered) return 565;
					else return 566;
				else if (Note == 9)
					if (Powered) return 567;
					else return 568;
				else if (Note == 10)
					if (Powered) return 569;
					else return 570;
				else if (Note == 11)
					if (Powered) return 571;
					else return 572;
				else if (Note == 12)
					if (Powered) return 573;
					else return 574;
				else if (Note == 13)
					if (Powered) return 575;
					else return 576;
				else if (Note == 14)
					if (Powered) return 577;
					else return 578;
				else if (Note == 15)
					if (Powered) return 579;
					else return 580;
				else if (Note == 16)
					if (Powered) return 581;
					else return 582;
				else if (Note == 17)
					if (Powered) return 583;
					else return 584;
				else if (Note == 18)
					if (Powered) return 585;
					else return 586;
				else if (Note == 19)
					if (Powered) return 587;
					else return 588;
				else if (Note == 20)
					if (Powered) return 589;
					else return 590;
				else if (Note == 21)
					if (Powered) return 591;
					else return 592;
				else if (Note == 22)
					if (Powered) return 593;
					else return 594;
				else if (Note == 23)
					if (Powered) return 595;
					else return 596;
				else
					if (Powered) return 597;
					else return 598;
			else if (Instrument == Instrument::Guitar)
				if (Note == 0)
					if (Powered) return 599;
					else return 600;
				else if (Note == 1)
					if (Powered) return 601;
					else return 602;
				else if (Note == 2)
					if (Powered) return 603;
					else return 604;
				else if (Note == 3)
					if (Powered) return 605;
					else return 606;
				else if (Note == 4)
					if (Powered) return 607;
					else return 608;
				else if (Note == 5)
					if (Powered) return 609;
					else return 610;
				else if (Note == 6)
					if (Powered) return 611;
					else return 612;
				else if (Note == 7)
					if (Powered) return 613;
					else return 614;
				else if (Note == 8)
					if (Powered) return 615;
					else return 616;
				else if (Note == 9)
					if (Powered) return 617;
					else return 618;
				else if (Note == 10)
					if (Powered) return 619;
					else return 620;
				else if (Note == 11)
					if (Powered) return 621;
					else return 622;
				else if (Note == 12)
					if (Powered) return 623;
					else return 624;
				else if (Note == 13)
					if (Powered) return 625;
					else return 626;
				else if (Note == 14)
					if (Powered) return 627;
					else return 628;
				else if (Note == 15)
					if (Powered) return 629;
					else return 630;
				else if (Note == 16)
					if (Powered) return 631;
					else return 632;
				else if (Note == 17)
					if (Powered) return 633;
					else return 634;
				else if (Note == 18)
					if (Powered) return 635;
					else return 636;
				else if (Note == 19)
					if (Powered) return 637;
					else return 638;
				else if (Note == 20)
					if (Powered) return 639;
					else return 640;
				else if (Note == 21)
					if (Powered) return 641;
					else return 642;
				else if (Note == 22)
					if (Powered) return 643;
					else return 644;
				else if (Note == 23)
					if (Powered) return 645;
					else return 646;
				else
					if (Powered) return 647;
					else return 648;
			else if (Instrument == Instrument::Chime)
				if (Note == 0)
					if (Powered) return 649;
					else return 650;
				else if (Note == 1)
					if (Powered) return 651;
					else return 652;
				else if (Note == 2)
					if (Powered) return 653;
					else return 654;
				else if (Note == 3)
					if (Powered) return 655;
					else return 656;
				else if (Note == 4)
					if (Powered) return 657;
					else return 658;
				else if (Note == 5)
					if (Powered) return 659;
					else return 660;
				else if (Note == 6)
					if (Powered) return 661;
					else return 662;
				else if (Note == 7)
					if (Powered) return 663;
					else return 664;
				else if (Note == 8)
					if (Powered) return 665;
					else return 666;
				else if (Note == 9)
					if (Powered) return 667;
					else return 668;
				else if (Note == 10)
					if (Powered) return 669;
					else return 670;
				else if (Note == 11)
					if (Powered) return 671;
					else return 672;
				else if (Note == 12)
					if (Powered) return 673;
					else return 674;
				else if (Note == 13)
					if (Powered) return 675;
					else return 676;
				else if (Note == 14)
					if (Powered) return 677;
					else return 678;
				else if (Note == 15)
					if (Powered) return 679;
					else return 680;
				else if (Note == 16)
					if (Powered) return 681;
					else return 682;
				else if (Note == 17)
					if (Powered) return 683;
					else return 684;
				else if (Note == 18)
					if (Powered) return 685;
					else return 686;
				else if (Note == 19)
					if (Powered) return 687;
					else return 688;
				else if (Note == 20)
					if (Powered) return 689;
					else return 690;
				else if (Note == 21)
					if (Powered) return 691;
					else return 692;
				else if (Note == 22)
					if (Powered) return 693;
					else return 694;
				else if (Note == 23)
					if (Powered) return 695;
					else return 696;
				else
					if (Powered) return 697;
					else return 698;
			else if (Instrument == Instrument::Xylophone)
				if (Note == 0)
					if (Powered) return 699;
					else return 700;
				else if (Note == 1)
					if (Powered) return 701;
					else return 702;
				else if (Note == 2)
					if (Powered) return 703;
					else return 704;
				else if (Note == 3)
					if (Powered) return 705;
					else return 706;
				else if (Note == 4)
					if (Powered) return 707;
					else return 708;
				else if (Note == 5)
					if (Powered) return 709;
					else return 710;
				else if (Note == 6)
					if (Powered) return 711;
					else return 712;
				else if (Note == 7)
					if (Powered) return 713;
					else return 714;
				else if (Note == 8)
					if (Powered) return 715;
					else return 716;
				else if (Note == 9)
					if (Powered) return 717;
					else return 718;
				else if (Note == 10)
					if (Powered) return 719;
					else return 720;
				else if (Note == 11)
					if (Powered) return 721;
					else return 722;
				else if (Note == 12)
					if (Powered) return 723;
					else return 724;
				else if (Note == 13)
					if (Powered) return 725;
					else return 726;
				else if (Note == 14)
					if (Powered) return 727;
					else return 728;
				else if (Note == 15)
					if (Powered) return 729;
					else return 730;
				else if (Note == 16)
					if (Powered) return 731;
					else return 732;
				else if (Note == 17)
					if (Powered) return 733;
					else return 734;
				else if (Note == 18)
					if (Powered) return 735;
					else return 736;
				else if (Note == 19)
					if (Powered) return 737;
					else return 738;
				else if (Note == 20)
					if (Powered) return 739;
					else return 740;
				else if (Note == 21)
					if (Powered) return 741;
					else return 742;
				else if (Note == 22)
					if (Powered) return 743;
					else return 744;
				else if (Note == 23)
					if (Powered) return 745;
					else return 746;
				else
					if (Powered) return 747;
					else return 748;
			else if (Instrument == Instrument::IronXylophone)
				if (Note == 0)
					if (Powered) return 749;
					else return 750;
				else if (Note == 1)
					if (Powered) return 751;
					else return 752;
				else if (Note == 2)
					if (Powered) return 753;
					else return 754;
				else if (Note == 3)
					if (Powered) return 755;
					else return 756;
				else if (Note == 4)
					if (Powered) return 757;
					else return 758;
				else if (Note == 5)
					if (Powered) return 759;
					else return 760;
				else if (Note == 6)
					if (Powered) return 761;
					else return 762;
				else if (Note == 7)
					if (Powered) return 763;
					else return 764;
				else if (Note == 8)
					if (Powered) return 765;
					else return 766;
				else if (Note == 9)
					if (Powered) return 767;
					else return 768;
				else if (Note == 10)
					if (Powered) return 769;
					else return 770;
				else if (Note == 11)
					if (Powered) return 771;
					else return 772;
				else if (Note == 12)
					if (Powered) return 773;
					else return 774;
				else if (Note == 13)
					if (Powered) return 775;
					else return 776;
				else if (Note == 14)
					if (Powered) return 777;
					else return 778;
				else if (Note == 15)
					if (Powered) return 779;
					else return 780;
				else if (Note == 16)
					if (Powered) return 781;
					else return 782;
				else if (Note == 17)
					if (Powered) return 783;
					else return 784;
				else if (Note == 18)
					if (Powered) return 785;
					else return 786;
				else if (Note == 19)
					if (Powered) return 787;
					else return 788;
				else if (Note == 20)
					if (Powered) return 789;
					else return 790;
				else if (Note == 21)
					if (Powered) return 791;
					else return 792;
				else if (Note == 22)
					if (Powered) return 793;
					else return 794;
				else if (Note == 23)
					if (Powered) return 795;
					else return 796;
				else
					if (Powered) return 797;
					else return 798;
			else if (Instrument == Instrument::CowBell)
				if (Note == 0)
					if (Powered) return 799;
					else return 800;
				else if (Note == 1)
					if (Powered) return 801;
					else return 802;
				else if (Note == 2)
					if (Powered) return 803;
					else return 804;
				else if (Note == 3)
					if (Powered) return 805;
					else return 806;
				else if (Note == 4)
					if (Powered) return 807;
					else return 808;
				else if (Note == 5)
					if (Powered) return 809;
					else return 810;
				else if (Note == 6)
					if (Powered) return 811;
					else return 812;
				else if (Note == 7)
					if (Powered) return 813;
					else return 814;
				else if (Note == 8)
					if (Powered) return 815;
					else return 816;
				else if (Note == 9)
					if (Powered) return 817;
					else return 818;
				else if (Note == 10)
					if (Powered) return 819;
					else return 820;
				else if (Note == 11)
					if (Powered) return 821;
					else return 822;
				else if (Note == 12)
					if (Powered) return 823;
					else return 824;
				else if (Note == 13)
					if (Powered) return 825;
					else return 826;
				else if (Note == 14)
					if (Powered) return 827;
					else return 828;
				else if (Note == 15)
					if (Powered) return 829;
					else return 830;
				else if (Note == 16)
					if (Powered) return 831;
					else return 832;
				else if (Note == 17)
					if (Powered) return 833;
					else return 834;
				else if (Note == 18)
					if (Powered) return 835;
					else return 836;
				else if (Note == 19)
					if (Powered) return 837;
					else return 838;
				else if (Note == 20)
					if (Powered) return 839;
					else return 840;
				else if (Note == 21)
					if (Powered) return 841;
					else return 842;
				else if (Note == 22)
					if (Powered) return 843;
					else return 844;
				else if (Note == 23)
					if (Powered) return 845;
					else return 846;
				else
					if (Powered) return 847;
					else return 848;
			else if (Instrument == Instrument::Didgeridoo)
				if (Note == 0)
					if (Powered) return 849;
					else return 850;
				else if (Note == 1)
					if (Powered) return 851;
					else return 852;
				else if (Note == 2)
					if (Powered) return 853;
					else return 854;
				else if (Note == 3)
					if (Powered) return 855;
					else return 856;
				else if (Note == 4)
					if (Powered) return 857;
					else return 858;
				else if (Note == 5)
					if (Powered) return 859;
					else return 860;
				else if (Note == 6)
					if (Powered) return 861;
					else return 862;
				else if (Note == 7)
					if (Powered) return 863;
					else return 864;
				else if (Note == 8)
					if (Powered) return 865;
					else return 866;
				else if (Note == 9)
					if (Powered) return 867;
					else return 868;
				else if (Note == 10)
					if (Powered) return 869;
					else return 870;
				else if (Note == 11)
					if (Powered) return 871;
					else return 872;
				else if (Note == 12)
					if (Powered) return 873;
					else return 874;
				else if (Note == 13)
					if (Powered) return 875;
					else return 876;
				else if (Note == 14)
					if (Powered) return 877;
					else return 878;
				else if (Note == 15)
					if (Powered) return 879;
					else return 880;
				else if (Note == 16)
					if (Powered) return 881;
					else return 882;
				else if (Note == 17)
					if (Powered) return 883;
					else return 884;
				else if (Note == 18)
					if (Powered) return 885;
					else return 886;
				else if (Note == 19)
					if (Powered) return 887;
					else return 888;
				else if (Note == 20)
					if (Powered) return 889;
					else return 890;
				else if (Note == 21)
					if (Powered) return 891;
					else return 892;
				else if (Note == 22)
					if (Powered) return 893;
					else return 894;
				else if (Note == 23)
					if (Powered) return 895;
					else return 896;
				else
					if (Powered) return 897;
					else return 898;
			else if (Instrument == Instrument::Bit)
				if (Note == 0)
					if (Powered) return 899;
					else return 900;
				else if (Note == 1)
					if (Powered) return 901;
					else return 902;
				else if (Note == 2)
					if (Powered) return 903;
					else return 904;
				else if (Note == 3)
					if (Powered) return 905;
					else return 906;
				else if (Note == 4)
					if (Powered) return 907;
					else return 908;
				else if (Note == 5)
					if (Powered) return 909;
					else return 910;
				else if (Note == 6)
					if (Powered) return 911;
					else return 912;
				else if (Note == 7)
					if (Powered) return 913;
					else return 914;
				else if (Note == 8)
					if (Powered) return 915;
					else return 916;
				else if (Note == 9)
					if (Powered) return 917;
					else return 918;
				else if (Note == 10)
					if (Powered) return 919;
					else return 920;
				else if (Note == 11)
					if (Powered) return 921;
					else return 922;
				else if (Note == 12)
					if (Powered) return 923;
					else return 924;
				else if (Note == 13)
					if (Powered) return 925;
					else return 926;
				else if (Note == 14)
					if (Powered) return 927;
					else return 928;
				else if (Note == 15)
					if (Powered) return 929;
					else return 930;
				else if (Note == 16)
					if (Powered) return 931;
					else return 932;
				else if (Note == 17)
					if (Powered) return 933;
					else return 934;
				else if (Note == 18)
					if (Powered) return 935;
					else return 936;
				else if (Note == 19)
					if (Powered) return 937;
					else return 938;
				else if (Note == 20)
					if (Powered) return 939;
					else return 940;
				else if (Note == 21)
					if (Powered) return 941;
					else return 942;
				else if (Note == 22)
					if (Powered) return 943;
					else return 944;
				else if (Note == 23)
					if (Powered) return 945;
					else return 946;
				else
					if (Powered) return 947;
					else return 948;
			else if (Instrument == Instrument::Banjo)
				if (Note == 0)
					if (Powered) return 949;
					else return 950;
				else if (Note == 1)
					if (Powered) return 951;
					else return 952;
				else if (Note == 2)
					if (Powered) return 953;
					else return 954;
				else if (Note == 3)
					if (Powered) return 955;
					else return 956;
				else if (Note == 4)
					if (Powered) return 957;
					else return 958;
				else if (Note == 5)
					if (Powered) return 959;
					else return 960;
				else if (Note == 6)
					if (Powered) return 961;
					else return 962;
				else if (Note == 7)
					if (Powered) return 963;
					else return 964;
				else if (Note == 8)
					if (Powered) return 965;
					else return 966;
				else if (Note == 9)
					if (Powered) return 967;
					else return 968;
				else if (Note == 10)
					if (Powered) return 969;
					else return 970;
				else if (Note == 11)
					if (Powered) return 971;
					else return 972;
				else if (Note == 12)
					if (Powered) return 973;
					else return 974;
				else if (Note == 13)
					if (Powered) return 975;
					else return 976;
				else if (Note == 14)
					if (Powered) return 977;
					else return 978;
				else if (Note == 15)
					if (Powered) return 979;
					else return 980;
				else if (Note == 16)
					if (Powered) return 981;
					else return 982;
				else if (Note == 17)
					if (Powered) return 983;
					else return 984;
				else if (Note == 18)
					if (Powered) return 985;
					else return 986;
				else if (Note == 19)
					if (Powered) return 987;
					else return 988;
				else if (Note == 20)
					if (Powered) return 989;
					else return 990;
				else if (Note == 21)
					if (Powered) return 991;
					else return 992;
				else if (Note == 22)
					if (Powered) return 993;
					else return 994;
				else if (Note == 23)
					if (Powered) return 995;
					else return 996;
				else
					if (Powered) return 997;
					else return 998;
			else
				if (Note == 0)
					if (Powered) return 999;
					else return 1000;
				else if (Note == 1)
					if (Powered) return 1001;
					else return 1002;
				else if (Note == 2)
					if (Powered) return 1003;
					else return 1004;
				else if (Note == 3)
					if (Powered) return 1005;
					else return 1006;
				else if (Note == 4)
					if (Powered) return 1007;
					else return 1008;
				else if (Note == 5)
					if (Powered) return 1009;
					else return 1010;
				else if (Note == 6)
					if (Powered) return 1011;
					else return 1012;
				else if (Note == 7)
					if (Powered) return 1013;
					else return 1014;
				else if (Note == 8)
					if (Powered) return 1015;
					else return 1016;
				else if (Note == 9)
					if (Powered) return 1017;
					else return 1018;
				else if (Note == 10)
					if (Powered) return 1019;
					else return 1020;
				else if (Note == 11)
					if (Powered) return 1021;
					else return 1022;
				else if (Note == 12)
					if (Powered) return 1023;
					else return 1024;
				else if (Note == 13)
					if (Powered) return 1025;
					else return 1026;
				else if (Note == 14)
					if (Powered) return 1027;
					else return 1028;
				else if (Note == 15)
					if (Powered) return 1029;
					else return 1030;
				else if (Note == 16)
					if (Powered) return 1031;
					else return 1032;
				else if (Note == 17)
					if (Powered) return 1033;
					else return 1034;
				else if (Note == 18)
					if (Powered) return 1035;
					else return 1036;
				else if (Note == 19)
					if (Powered) return 1037;
					else return 1038;
				else if (Note == 20)
					if (Powered) return 1039;
					else return 1040;
				else if (Note == 21)
					if (Powered) return 1041;
					else return 1042;
				else if (Note == 22)
					if (Powered) return 1043;
					else return 1044;
				else if (Note == 23)
					if (Powered) return 1045;
					else return 1046;
				else
					if (Powered) return 1047;
					else return 1048;
		}
		BlockState NoteBlock();
		enum Instrument Instrument(BlockState Block);
		unsigned char Note(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace OakButton
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState OakButton(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6346;
					else return 6347;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6348;
					else return 6349;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6350;
					else return 6351;
				else
					if (Powered) return 6352;
					else return 6353;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6354;
					else return 6355;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6356;
					else return 6357;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6358;
					else return 6359;
				else
					if (Powered) return 6360;
					else return 6361;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6362;
					else return 6363;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6364;
					else return 6365;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6366;
					else return 6367;
				else
					if (Powered) return 6368;
					else return 6369;
		}
		BlockState OakButton();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace OakDoor
	{
		enum class Half
		{
			Upper,
			Lower
		};
		enum class Hinge
		{
			Left,
			Right
		};
		constexpr BlockState OakDoor(const eBlockFace Facing, const enum Half Half, const enum Hinge Hinge, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3573;
							else return 3574;
						else
							if (Powered) return 3575;
							else return 3576;
					else
						if (Open)
							if (Powered) return 3577;
							else return 3578;
						else
							if (Powered) return 3579;
							else return 3580;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3581;
							else return 3582;
						else
							if (Powered) return 3583;
							else return 3584;
					else
						if (Open)
							if (Powered) return 3585;
							else return 3586;
						else
							if (Powered) return 3587;
							else return 3588;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3589;
							else return 3590;
						else
							if (Powered) return 3591;
							else return 3592;
					else
						if (Open)
							if (Powered) return 3593;
							else return 3594;
						else
							if (Powered) return 3595;
							else return 3596;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3597;
							else return 3598;
						else
							if (Powered) return 3599;
							else return 3600;
					else
						if (Open)
							if (Powered) return 3601;
							else return 3602;
						else
							if (Powered) return 3603;
							else return 3604;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3605;
							else return 3606;
						else
							if (Powered) return 3607;
							else return 3608;
					else
						if (Open)
							if (Powered) return 3609;
							else return 3610;
						else
							if (Powered) return 3611;
							else return 3612;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3613;
							else return 3614;
						else
							if (Powered) return 3615;
							else return 3616;
					else
						if (Open)
							if (Powered) return 3617;
							else return 3618;
						else
							if (Powered) return 3619;
							else return 3620;
			else
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3621;
							else return 3622;
						else
							if (Powered) return 3623;
							else return 3624;
					else
						if (Open)
							if (Powered) return 3625;
							else return 3626;
						else
							if (Powered) return 3627;
							else return 3628;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 3629;
							else return 3630;
						else
							if (Powered) return 3631;
							else return 3632;
					else
						if (Open)
							if (Powered) return 3633;
							else return 3634;
						else
							if (Powered) return 3635;
							else return 3636;
		}
		BlockState OakDoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Hinge Hinge(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace OakFence
	{
		constexpr BlockState OakFence(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 3968;
						else return 3969;
					else
						if (West) return 3972;
						else return 3973;
				else
					if (South)
						if (West) return 3976;
						else return 3977;
					else
						if (West) return 3980;
						else return 3981;
			else
				if (North)
					if (South)
						if (West) return 3984;
						else return 3985;
					else
						if (West) return 3988;
						else return 3989;
				else
					if (South)
						if (West) return 3992;
						else return 3993;
					else
						if (West) return 3996;
						else return 3997;
		}
		BlockState OakFence();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace OakFenceGate
	{
		constexpr BlockState OakFenceGate(const eBlockFace Facing, const bool InWall, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (InWall)
					if (Open)
						if (Powered) return 4820;
						else return 4821;
					else
						if (Powered) return 4822;
						else return 4823;
				else
					if (Open)
						if (Powered) return 4824;
						else return 4825;
					else
						if (Powered) return 4826;
						else return 4827;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (InWall)
					if (Open)
						if (Powered) return 4828;
						else return 4829;
					else
						if (Powered) return 4830;
						else return 4831;
				else
					if (Open)
						if (Powered) return 4832;
						else return 4833;
					else
						if (Powered) return 4834;
						else return 4835;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (InWall)
					if (Open)
						if (Powered) return 4836;
						else return 4837;
					else
						if (Powered) return 4838;
						else return 4839;
				else
					if (Open)
						if (Powered) return 4840;
						else return 4841;
					else
						if (Powered) return 4842;
						else return 4843;
			else
				if (InWall)
					if (Open)
						if (Powered) return 4844;
						else return 4845;
					else
						if (Powered) return 4846;
						else return 4847;
				else
					if (Open)
						if (Powered) return 4848;
						else return 4849;
					else
						if (Powered) return 4850;
						else return 4851;
		}
		BlockState OakFenceGate();
		eBlockFace Facing(BlockState Block);
		bool InWall(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace OakLeaves
	{
		constexpr BlockState OakLeaves(const unsigned char Distance, const bool Persistent)
		{
			if (Distance == 1)
				if (Persistent) return 145;
				else return 146;
			else if (Distance == 2)
				if (Persistent) return 147;
				else return 148;
			else if (Distance == 3)
				if (Persistent) return 149;
				else return 150;
			else if (Distance == 4)
				if (Persistent) return 151;
				else return 152;
			else if (Distance == 5)
				if (Persistent) return 153;
				else return 154;
			else if (Distance == 6)
				if (Persistent) return 155;
				else return 156;
			else
				if (Persistent) return 157;
				else return 158;
		}
		BlockState OakLeaves();
		unsigned char Distance(BlockState Block);
		bool Persistent(BlockState Block);
	}
	namespace OakLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState OakLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 73;
			else if (Axis == Axis::Y) return 74;
			else return 75;
		}
		BlockState OakLog();
		enum Axis Axis(BlockState Block);
	}
	namespace OakPlanks
	{
		constexpr BlockState OakPlanks()
		{
			return 15;
		}
	}
	namespace OakPressurePlate
	{
		constexpr BlockState OakPressurePlate(const bool Powered)
		{
			if (Powered) return 3873;
			else return 3874;
		}
		BlockState OakPressurePlate();
		bool Powered(BlockState Block);
	}
	namespace OakSapling
	{
		constexpr BlockState OakSapling(const unsigned char Stage)
		{
			if (Stage == 0) return 21;
			else return 22;
		}
		BlockState OakSapling();
		unsigned char Stage(BlockState Block);
	}
	namespace OakSign
	{
		constexpr BlockState OakSign(const unsigned char Rotation)
		{
			if (Rotation == 0) return 3382;
			else if (Rotation == 1) return 3384;
			else if (Rotation == 2) return 3386;
			else if (Rotation == 3) return 3388;
			else if (Rotation == 4) return 3390;
			else if (Rotation == 5) return 3392;
			else if (Rotation == 6) return 3394;
			else if (Rotation == 7) return 3396;
			else if (Rotation == 8) return 3398;
			else if (Rotation == 9) return 3400;
			else if (Rotation == 10) return 3402;
			else if (Rotation == 11) return 3404;
			else if (Rotation == 12) return 3406;
			else if (Rotation == 13) return 3408;
			else if (Rotation == 14) return 3410;
			else return 3412;
		}
		BlockState OakSign();
		unsigned char Rotation(BlockState Block);
	}
	namespace OakSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState OakSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8301;
			else if (Type == Type::Bottom) return 8303;
			else return 8305;
		}
		BlockState OakSlab();
		enum Type Type(BlockState Block);
	}
	namespace OakStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState OakStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 1955;
					else if (Shape == Shape::InnerLeft) return 1957;
					else if (Shape == Shape::InnerRight) return 1959;
					else if (Shape == Shape::OuterLeft) return 1961;
					else return 1963;
				else
					if (Shape == Shape::Straight) return 1965;
					else if (Shape == Shape::InnerLeft) return 1967;
					else if (Shape == Shape::InnerRight) return 1969;
					else if (Shape == Shape::OuterLeft) return 1971;
					else return 1973;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 1975;
					else if (Shape == Shape::InnerLeft) return 1977;
					else if (Shape == Shape::InnerRight) return 1979;
					else if (Shape == Shape::OuterLeft) return 1981;
					else return 1983;
				else
					if (Shape == Shape::Straight) return 1985;
					else if (Shape == Shape::InnerLeft) return 1987;
					else if (Shape == Shape::InnerRight) return 1989;
					else if (Shape == Shape::OuterLeft) return 1991;
					else return 1993;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 1995;
					else if (Shape == Shape::InnerLeft) return 1997;
					else if (Shape == Shape::InnerRight) return 1999;
					else if (Shape == Shape::OuterLeft) return 2001;
					else return 2003;
				else
					if (Shape == Shape::Straight) return 2005;
					else if (Shape == Shape::InnerLeft) return 2007;
					else if (Shape == Shape::InnerRight) return 2009;
					else if (Shape == Shape::OuterLeft) return 2011;
					else return 2013;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 2015;
					else if (Shape == Shape::InnerLeft) return 2017;
					else if (Shape == Shape::InnerRight) return 2019;
					else if (Shape == Shape::OuterLeft) return 2021;
					else return 2023;
				else
					if (Shape == Shape::Straight) return 2025;
					else if (Shape == Shape::InnerLeft) return 2027;
					else if (Shape == Shape::InnerRight) return 2029;
					else if (Shape == Shape::OuterLeft) return 2031;
					else return 2033;
		}
		BlockState OakStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace OakTrapdoor
	{
		enum class Half
		{
			Top,
			Bottom
		};
		constexpr BlockState OakTrapdoor(const eBlockFace Facing, const enum Half Half, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4112;
						else return 4114;
					else
						if (Powered) return 4116;
						else return 4118;
				else
					if (Open)
						if (Powered) return 4120;
						else return 4122;
					else
						if (Powered) return 4124;
						else return 4126;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4128;
						else return 4130;
					else
						if (Powered) return 4132;
						else return 4134;
				else
					if (Open)
						if (Powered) return 4136;
						else return 4138;
					else
						if (Powered) return 4140;
						else return 4142;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4144;
						else return 4146;
					else
						if (Powered) return 4148;
						else return 4150;
				else
					if (Open)
						if (Powered) return 4152;
						else return 4154;
					else
						if (Powered) return 4156;
						else return 4158;
			else
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4160;
						else return 4162;
					else
						if (Powered) return 4164;
						else return 4166;
				else
					if (Open)
						if (Powered) return 4168;
						else return 4170;
					else
						if (Powered) return 4172;
						else return 4174;
		}
		BlockState OakTrapdoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace OakWallSign
	{
		constexpr BlockState OakWallSign(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 3736;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 3738;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 3740;
			else return 3742;
		}
		BlockState OakWallSign();
		eBlockFace Facing(BlockState Block);
	}
	namespace OakWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState OakWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 109;
			else if (Axis == Axis::Y) return 110;
			else return 111;
		}
		BlockState OakWood();
		enum Axis Axis(BlockState Block);
	}
	namespace Observer
	{
		constexpr BlockState Observer(const eBlockFace Facing, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Powered) return 9260;
				else return 9261;
			else if (Facing == eBlockFace::BLOCK_FACE_XP)
				if (Powered) return 9262;
				else return 9263;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Powered) return 9264;
				else return 9265;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Powered) return 9266;
				else return 9267;
			else if (Facing == eBlockFace::BLOCK_FACE_YP)
				if (Powered) return 9268;
				else return 9269;
			else
				if (Powered) return 9270;
				else return 9271;
		}
		BlockState Observer();
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace Obsidian
	{
		constexpr BlockState Obsidian()
		{
			return 1434;
		}
	}
	namespace OrangeBanner
	{
		constexpr BlockState OrangeBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 7913;
			else if (Rotation == 1) return 7914;
			else if (Rotation == 2) return 7915;
			else if (Rotation == 3) return 7916;
			else if (Rotation == 4) return 7917;
			else if (Rotation == 5) return 7918;
			else if (Rotation == 6) return 7919;
			else if (Rotation == 7) return 7920;
			else if (Rotation == 8) return 7921;
			else if (Rotation == 9) return 7922;
			else if (Rotation == 10) return 7923;
			else if (Rotation == 11) return 7924;
			else if (Rotation == 12) return 7925;
			else if (Rotation == 13) return 7926;
			else if (Rotation == 14) return 7927;
			else return 7928;
		}
		BlockState OrangeBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace OrangeBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState OrangeBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1065;
					else return 1066;
				else
					if (Part == Part::Head) return 1067;
					else return 1068;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1069;
					else return 1070;
				else
					if (Part == Part::Head) return 1071;
					else return 1072;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1073;
					else return 1074;
				else
					if (Part == Part::Head) return 1075;
					else return 1076;
			else
				if (Occupied)
					if (Part == Part::Head) return 1077;
					else return 1078;
				else
					if (Part == Part::Head) return 1079;
					else return 1080;
		}
		BlockState OrangeBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace OrangeCarpet
	{
		constexpr BlockState OrangeCarpet()
		{
			return 7867;
		}
	}
	namespace OrangeConcrete
	{
		constexpr BlockState OrangeConcrete()
		{
			return 9439;
		}
	}
	namespace OrangeConcretePowder
	{
		constexpr BlockState OrangeConcretePowder()
		{
			return 9455;
		}
	}
	namespace OrangeGlazedTerracotta
	{
		constexpr BlockState OrangeGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9378;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9379;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9380;
			else return 9381;
		}
		BlockState OrangeGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace OrangeShulkerBox
	{
		constexpr BlockState OrangeShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9284;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9285;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9286;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9287;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9288;
			else return 9289;
		}
		BlockState OrangeShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace OrangeStainedGlass
	{
		constexpr BlockState OrangeStainedGlass()
		{
			return 4096;
		}
	}
	namespace OrangeStainedGlassPane
	{
		constexpr BlockState OrangeStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 6897;
						else return 6898;
					else
						if (West) return 6901;
						else return 6902;
				else
					if (South)
						if (West) return 6905;
						else return 6906;
					else
						if (West) return 6909;
						else return 6910;
			else
				if (North)
					if (South)
						if (West) return 6913;
						else return 6914;
					else
						if (West) return 6917;
						else return 6918;
				else
					if (South)
						if (West) return 6921;
						else return 6922;
					else
						if (West) return 6925;
						else return 6926;
		}
		BlockState OrangeStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace OrangeTerracotta
	{
		constexpr BlockState OrangeTerracotta()
		{
			return 6848;
		}
	}
	namespace OrangeTulip
	{
		constexpr BlockState OrangeTulip()
		{
			return 1418;
		}
	}
	namespace OrangeWallBanner
	{
		constexpr BlockState OrangeWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8157;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8158;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8159;
			else return 8160;
		}
		BlockState OrangeWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace OrangeWool
	{
		constexpr BlockState OrangeWool()
		{
			return 1385;
		}
	}
	namespace OxeyeDaisy
	{
		constexpr BlockState OxeyeDaisy()
		{
			return 1421;
		}
	}
	namespace PackedIce
	{
		constexpr BlockState PackedIce()
		{
			return 7884;
		}
	}
	namespace Peony
	{
		enum class Half
		{
			Upper,
			Lower
		};
		constexpr BlockState Peony(const enum Half Half)
		{
			if (Half == Half::Upper) return 7891;
			else return 7892;
		}
		BlockState Peony();
		enum Half Half(BlockState Block);
	}
	namespace PetrifiedOakSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState PetrifiedOakSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8361;
			else if (Type == Type::Bottom) return 8363;
			else return 8365;
		}
		BlockState PetrifiedOakSlab();
		enum Type Type(BlockState Block);
	}
	namespace PinkBanner
	{
		constexpr BlockState PinkBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 7993;
			else if (Rotation == 1) return 7994;
			else if (Rotation == 2) return 7995;
			else if (Rotation == 3) return 7996;
			else if (Rotation == 4) return 7997;
			else if (Rotation == 5) return 7998;
			else if (Rotation == 6) return 7999;
			else if (Rotation == 7) return 8000;
			else if (Rotation == 8) return 8001;
			else if (Rotation == 9) return 8002;
			else if (Rotation == 10) return 8003;
			else if (Rotation == 11) return 8004;
			else if (Rotation == 12) return 8005;
			else if (Rotation == 13) return 8006;
			else if (Rotation == 14) return 8007;
			else return 8008;
		}
		BlockState PinkBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace PinkBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState PinkBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1145;
					else return 1146;
				else
					if (Part == Part::Head) return 1147;
					else return 1148;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1149;
					else return 1150;
				else
					if (Part == Part::Head) return 1151;
					else return 1152;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1153;
					else return 1154;
				else
					if (Part == Part::Head) return 1155;
					else return 1156;
			else
				if (Occupied)
					if (Part == Part::Head) return 1157;
					else return 1158;
				else
					if (Part == Part::Head) return 1159;
					else return 1160;
		}
		BlockState PinkBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace PinkCarpet
	{
		constexpr BlockState PinkCarpet()
		{
			return 7872;
		}
	}
	namespace PinkConcrete
	{
		constexpr BlockState PinkConcrete()
		{
			return 9444;
		}
	}
	namespace PinkConcretePowder
	{
		constexpr BlockState PinkConcretePowder()
		{
			return 9460;
		}
	}
	namespace PinkGlazedTerracotta
	{
		constexpr BlockState PinkGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9398;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9399;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9400;
			else return 9401;
		}
		BlockState PinkGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace PinkShulkerBox
	{
		constexpr BlockState PinkShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9314;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9315;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9316;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9317;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9318;
			else return 9319;
		}
		BlockState PinkShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace PinkStainedGlass
	{
		constexpr BlockState PinkStainedGlass()
		{
			return 4101;
		}
	}
	namespace PinkStainedGlassPane
	{
		constexpr BlockState PinkStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7057;
						else return 7058;
					else
						if (West) return 7061;
						else return 7062;
				else
					if (South)
						if (West) return 7065;
						else return 7066;
					else
						if (West) return 7069;
						else return 7070;
			else
				if (North)
					if (South)
						if (West) return 7073;
						else return 7074;
					else
						if (West) return 7077;
						else return 7078;
				else
					if (South)
						if (West) return 7081;
						else return 7082;
					else
						if (West) return 7085;
						else return 7086;
		}
		BlockState PinkStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace PinkTerracotta
	{
		constexpr BlockState PinkTerracotta()
		{
			return 6853;
		}
	}
	namespace PinkTulip
	{
		constexpr BlockState PinkTulip()
		{
			return 1420;
		}
	}
	namespace PinkWallBanner
	{
		constexpr BlockState PinkWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8177;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8178;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8179;
			else return 8180;
		}
		BlockState PinkWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace PinkWool
	{
		constexpr BlockState PinkWool()
		{
			return 1390;
		}
	}
	namespace Piston
	{
		constexpr BlockState Piston(const bool Extended, const eBlockFace Facing)
		{
			if (Extended)
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 1348;
				else if (Facing == eBlockFace::BLOCK_FACE_XP) return 1349;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 1350;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 1351;
				else if (Facing == eBlockFace::BLOCK_FACE_YP) return 1352;
				else return 1353;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 1354;
				else if (Facing == eBlockFace::BLOCK_FACE_XP) return 1355;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 1356;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 1357;
				else if (Facing == eBlockFace::BLOCK_FACE_YP) return 1358;
				else return 1359;
		}
		BlockState Piston();
		bool Extended(BlockState Block);
		eBlockFace Facing(BlockState Block);
	}
	namespace PistonHead
	{
		enum class Type
		{
			Normal,
			Sticky
		};
		constexpr BlockState PistonHead(const eBlockFace Facing, const bool Short, const enum Type Type)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Short)
					if (Type == Type::Normal) return 1360;
					else return 1361;
				else
					if (Type == Type::Normal) return 1362;
					else return 1363;
			else if (Facing == eBlockFace::BLOCK_FACE_XP)
				if (Short)
					if (Type == Type::Normal) return 1364;
					else return 1365;
				else
					if (Type == Type::Normal) return 1366;
					else return 1367;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Short)
					if (Type == Type::Normal) return 1368;
					else return 1369;
				else
					if (Type == Type::Normal) return 1370;
					else return 1371;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Short)
					if (Type == Type::Normal) return 1372;
					else return 1373;
				else
					if (Type == Type::Normal) return 1374;
					else return 1375;
			else if (Facing == eBlockFace::BLOCK_FACE_YP)
				if (Short)
					if (Type == Type::Normal) return 1376;
					else return 1377;
				else
					if (Type == Type::Normal) return 1378;
					else return 1379;
			else
				if (Short)
					if (Type == Type::Normal) return 1380;
					else return 1381;
				else
					if (Type == Type::Normal) return 1382;
					else return 1383;
		}
		BlockState PistonHead();
		eBlockFace Facing(BlockState Block);
		bool Short(BlockState Block);
		enum Type Type(BlockState Block);
	}
	namespace PlayerHead
	{
		constexpr BlockState PlayerHead(const unsigned char Rotation)
		{
			if (Rotation == 0) return 6550;
			else if (Rotation == 1) return 6551;
			else if (Rotation == 2) return 6552;
			else if (Rotation == 3) return 6553;
			else if (Rotation == 4) return 6554;
			else if (Rotation == 5) return 6555;
			else if (Rotation == 6) return 6556;
			else if (Rotation == 7) return 6557;
			else if (Rotation == 8) return 6558;
			else if (Rotation == 9) return 6559;
			else if (Rotation == 10) return 6560;
			else if (Rotation == 11) return 6561;
			else if (Rotation == 12) return 6562;
			else if (Rotation == 13) return 6563;
			else if (Rotation == 14) return 6564;
			else return 6565;
		}
		BlockState PlayerHead();
		unsigned char Rotation(BlockState Block);
	}
	namespace PlayerWallHead
	{
		constexpr BlockState PlayerWallHead(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6566;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6567;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6568;
			else return 6569;
		}
		BlockState PlayerWallHead();
		eBlockFace Facing(BlockState Block);
	}
	namespace Podzol
	{
		constexpr BlockState Podzol(const bool Snowy)
		{
			if (Snowy) return 12;
			else return 13;
		}
		BlockState Podzol();
		bool Snowy(BlockState Block);
	}
	namespace PolishedAndesite
	{
		constexpr BlockState PolishedAndesite()
		{
			return 7;
		}
	}
	namespace PolishedAndesiteSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState PolishedAndesiteSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10856;
			else if (Type == Type::Bottom) return 10858;
			else return 10860;
		}
		BlockState PolishedAndesiteSlab();
		enum Type Type(BlockState Block);
	}
	namespace PolishedAndesiteStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState PolishedAndesiteStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10630;
					else if (Shape == Shape::InnerLeft) return 10632;
					else if (Shape == Shape::InnerRight) return 10634;
					else if (Shape == Shape::OuterLeft) return 10636;
					else return 10638;
				else
					if (Shape == Shape::Straight) return 10640;
					else if (Shape == Shape::InnerLeft) return 10642;
					else if (Shape == Shape::InnerRight) return 10644;
					else if (Shape == Shape::OuterLeft) return 10646;
					else return 10648;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10650;
					else if (Shape == Shape::InnerLeft) return 10652;
					else if (Shape == Shape::InnerRight) return 10654;
					else if (Shape == Shape::OuterLeft) return 10656;
					else return 10658;
				else
					if (Shape == Shape::Straight) return 10660;
					else if (Shape == Shape::InnerLeft) return 10662;
					else if (Shape == Shape::InnerRight) return 10664;
					else if (Shape == Shape::OuterLeft) return 10666;
					else return 10668;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10670;
					else if (Shape == Shape::InnerLeft) return 10672;
					else if (Shape == Shape::InnerRight) return 10674;
					else if (Shape == Shape::OuterLeft) return 10676;
					else return 10678;
				else
					if (Shape == Shape::Straight) return 10680;
					else if (Shape == Shape::InnerLeft) return 10682;
					else if (Shape == Shape::InnerRight) return 10684;
					else if (Shape == Shape::OuterLeft) return 10686;
					else return 10688;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10690;
					else if (Shape == Shape::InnerLeft) return 10692;
					else if (Shape == Shape::InnerRight) return 10694;
					else if (Shape == Shape::OuterLeft) return 10696;
					else return 10698;
				else
					if (Shape == Shape::Straight) return 10700;
					else if (Shape == Shape::InnerLeft) return 10702;
					else if (Shape == Shape::InnerRight) return 10704;
					else if (Shape == Shape::OuterLeft) return 10706;
					else return 10708;
		}
		BlockState PolishedAndesiteStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace PolishedBasalt
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState PolishedBasalt(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 4005;
			else if (Axis == Axis::Y) return 4006;
			else return 4007;
		}
		BlockState PolishedBasalt();
		enum Axis Axis(BlockState Block);
	}
	namespace PolishedBlackstone
	{
		constexpr BlockState PolishedBlackstone()
		{
			return 16250;
		}
	}
	namespace PolishedBlackstoneBrickSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState PolishedBlackstoneBrickSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 16255;
			else if (Type == Type::Bottom) return 16257;
			else return 16259;
		}
		BlockState PolishedBlackstoneBrickSlab();
		enum Type Type(BlockState Block);
	}
	namespace PolishedBlackstoneBrickStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState PolishedBlackstoneBrickStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 16261;
					else if (Shape == Shape::InnerLeft) return 16263;
					else if (Shape == Shape::InnerRight) return 16265;
					else if (Shape == Shape::OuterLeft) return 16267;
					else return 16269;
				else
					if (Shape == Shape::Straight) return 16271;
					else if (Shape == Shape::InnerLeft) return 16273;
					else if (Shape == Shape::InnerRight) return 16275;
					else if (Shape == Shape::OuterLeft) return 16277;
					else return 16279;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 16281;
					else if (Shape == Shape::InnerLeft) return 16283;
					else if (Shape == Shape::InnerRight) return 16285;
					else if (Shape == Shape::OuterLeft) return 16287;
					else return 16289;
				else
					if (Shape == Shape::Straight) return 16291;
					else if (Shape == Shape::InnerLeft) return 16293;
					else if (Shape == Shape::InnerRight) return 16295;
					else if (Shape == Shape::OuterLeft) return 16297;
					else return 16299;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 16301;
					else if (Shape == Shape::InnerLeft) return 16303;
					else if (Shape == Shape::InnerRight) return 16305;
					else if (Shape == Shape::OuterLeft) return 16307;
					else return 16309;
				else
					if (Shape == Shape::Straight) return 16311;
					else if (Shape == Shape::InnerLeft) return 16313;
					else if (Shape == Shape::InnerRight) return 16315;
					else if (Shape == Shape::OuterLeft) return 16317;
					else return 16319;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 16321;
					else if (Shape == Shape::InnerLeft) return 16323;
					else if (Shape == Shape::InnerRight) return 16325;
					else if (Shape == Shape::OuterLeft) return 16327;
					else return 16329;
				else
					if (Shape == Shape::Straight) return 16331;
					else if (Shape == Shape::InnerLeft) return 16333;
					else if (Shape == Shape::InnerRight) return 16335;
					else if (Shape == Shape::OuterLeft) return 16337;
					else return 16339;
		}
		BlockState PolishedBlackstoneBrickStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace PolishedBlackstoneBrickWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState PolishedBlackstoneBrickWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16343;
							else if (West == West::Low) return 16344;
							else return 16345;
						else
							if (West == West::None) return 16349;
							else if (West == West::Low) return 16350;
							else return 16351;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16355;
							else if (West == West::Low) return 16356;
							else return 16357;
						else
							if (West == West::None) return 16361;
							else if (West == West::Low) return 16362;
							else return 16363;
					else
						if (Up)
							if (West == West::None) return 16367;
							else if (West == West::Low) return 16368;
							else return 16369;
						else
							if (West == West::None) return 16373;
							else if (West == West::Low) return 16374;
							else return 16375;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16379;
							else if (West == West::Low) return 16380;
							else return 16381;
						else
							if (West == West::None) return 16385;
							else if (West == West::Low) return 16386;
							else return 16387;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16391;
							else if (West == West::Low) return 16392;
							else return 16393;
						else
							if (West == West::None) return 16397;
							else if (West == West::Low) return 16398;
							else return 16399;
					else
						if (Up)
							if (West == West::None) return 16403;
							else if (West == West::Low) return 16404;
							else return 16405;
						else
							if (West == West::None) return 16409;
							else if (West == West::Low) return 16410;
							else return 16411;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16415;
							else if (West == West::Low) return 16416;
							else return 16417;
						else
							if (West == West::None) return 16421;
							else if (West == West::Low) return 16422;
							else return 16423;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16427;
							else if (West == West::Low) return 16428;
							else return 16429;
						else
							if (West == West::None) return 16433;
							else if (West == West::Low) return 16434;
							else return 16435;
					else
						if (Up)
							if (West == West::None) return 16439;
							else if (West == West::Low) return 16440;
							else return 16441;
						else
							if (West == West::None) return 16445;
							else if (West == West::Low) return 16446;
							else return 16447;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16451;
							else if (West == West::Low) return 16452;
							else return 16453;
						else
							if (West == West::None) return 16457;
							else if (West == West::Low) return 16458;
							else return 16459;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16463;
							else if (West == West::Low) return 16464;
							else return 16465;
						else
							if (West == West::None) return 16469;
							else if (West == West::Low) return 16470;
							else return 16471;
					else
						if (Up)
							if (West == West::None) return 16475;
							else if (West == West::Low) return 16476;
							else return 16477;
						else
							if (West == West::None) return 16481;
							else if (West == West::Low) return 16482;
							else return 16483;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16487;
							else if (West == West::Low) return 16488;
							else return 16489;
						else
							if (West == West::None) return 16493;
							else if (West == West::Low) return 16494;
							else return 16495;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16499;
							else if (West == West::Low) return 16500;
							else return 16501;
						else
							if (West == West::None) return 16505;
							else if (West == West::Low) return 16506;
							else return 16507;
					else
						if (Up)
							if (West == West::None) return 16511;
							else if (West == West::Low) return 16512;
							else return 16513;
						else
							if (West == West::None) return 16517;
							else if (West == West::Low) return 16518;
							else return 16519;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16523;
							else if (West == West::Low) return 16524;
							else return 16525;
						else
							if (West == West::None) return 16529;
							else if (West == West::Low) return 16530;
							else return 16531;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16535;
							else if (West == West::Low) return 16536;
							else return 16537;
						else
							if (West == West::None) return 16541;
							else if (West == West::Low) return 16542;
							else return 16543;
					else
						if (Up)
							if (West == West::None) return 16547;
							else if (West == West::Low) return 16548;
							else return 16549;
						else
							if (West == West::None) return 16553;
							else if (West == West::Low) return 16554;
							else return 16555;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16559;
							else if (West == West::Low) return 16560;
							else return 16561;
						else
							if (West == West::None) return 16565;
							else if (West == West::Low) return 16566;
							else return 16567;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16571;
							else if (West == West::Low) return 16572;
							else return 16573;
						else
							if (West == West::None) return 16577;
							else if (West == West::Low) return 16578;
							else return 16579;
					else
						if (Up)
							if (West == West::None) return 16583;
							else if (West == West::Low) return 16584;
							else return 16585;
						else
							if (West == West::None) return 16589;
							else if (West == West::Low) return 16590;
							else return 16591;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16595;
							else if (West == West::Low) return 16596;
							else return 16597;
						else
							if (West == West::None) return 16601;
							else if (West == West::Low) return 16602;
							else return 16603;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16607;
							else if (West == West::Low) return 16608;
							else return 16609;
						else
							if (West == West::None) return 16613;
							else if (West == West::Low) return 16614;
							else return 16615;
					else
						if (Up)
							if (West == West::None) return 16619;
							else if (West == West::Low) return 16620;
							else return 16621;
						else
							if (West == West::None) return 16625;
							else if (West == West::Low) return 16626;
							else return 16627;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16631;
							else if (West == West::Low) return 16632;
							else return 16633;
						else
							if (West == West::None) return 16637;
							else if (West == West::Low) return 16638;
							else return 16639;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16643;
							else if (West == West::Low) return 16644;
							else return 16645;
						else
							if (West == West::None) return 16649;
							else if (West == West::Low) return 16650;
							else return 16651;
					else
						if (Up)
							if (West == West::None) return 16655;
							else if (West == West::Low) return 16656;
							else return 16657;
						else
							if (West == West::None) return 16661;
							else if (West == West::Low) return 16662;
							else return 16663;
		}
		BlockState PolishedBlackstoneBrickWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace PolishedBlackstoneBricks
	{
		constexpr BlockState PolishedBlackstoneBricks()
		{
			return 16251;
		}
	}
	namespace PolishedBlackstoneButton
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState PolishedBlackstoneButton(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 16753;
					else return 16754;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 16755;
					else return 16756;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 16757;
					else return 16758;
				else
					if (Powered) return 16759;
					else return 16760;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 16761;
					else return 16762;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 16763;
					else return 16764;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 16765;
					else return 16766;
				else
					if (Powered) return 16767;
					else return 16768;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 16769;
					else return 16770;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 16771;
					else return 16772;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 16773;
					else return 16774;
				else
					if (Powered) return 16775;
					else return 16776;
		}
		BlockState PolishedBlackstoneButton();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace PolishedBlackstonePressurePlate
	{
		constexpr BlockState PolishedBlackstonePressurePlate(const bool Powered)
		{
			if (Powered) return 16751;
			else return 16752;
		}
		BlockState PolishedBlackstonePressurePlate();
		bool Powered(BlockState Block);
	}
	namespace PolishedBlackstoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState PolishedBlackstoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 16746;
			else if (Type == Type::Bottom) return 16748;
			else return 16750;
		}
		BlockState PolishedBlackstoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace PolishedBlackstoneStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState PolishedBlackstoneStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 16666;
					else if (Shape == Shape::InnerLeft) return 16668;
					else if (Shape == Shape::InnerRight) return 16670;
					else if (Shape == Shape::OuterLeft) return 16672;
					else return 16674;
				else
					if (Shape == Shape::Straight) return 16676;
					else if (Shape == Shape::InnerLeft) return 16678;
					else if (Shape == Shape::InnerRight) return 16680;
					else if (Shape == Shape::OuterLeft) return 16682;
					else return 16684;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 16686;
					else if (Shape == Shape::InnerLeft) return 16688;
					else if (Shape == Shape::InnerRight) return 16690;
					else if (Shape == Shape::OuterLeft) return 16692;
					else return 16694;
				else
					if (Shape == Shape::Straight) return 16696;
					else if (Shape == Shape::InnerLeft) return 16698;
					else if (Shape == Shape::InnerRight) return 16700;
					else if (Shape == Shape::OuterLeft) return 16702;
					else return 16704;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 16706;
					else if (Shape == Shape::InnerLeft) return 16708;
					else if (Shape == Shape::InnerRight) return 16710;
					else if (Shape == Shape::OuterLeft) return 16712;
					else return 16714;
				else
					if (Shape == Shape::Straight) return 16716;
					else if (Shape == Shape::InnerLeft) return 16718;
					else if (Shape == Shape::InnerRight) return 16720;
					else if (Shape == Shape::OuterLeft) return 16722;
					else return 16724;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 16726;
					else if (Shape == Shape::InnerLeft) return 16728;
					else if (Shape == Shape::InnerRight) return 16730;
					else if (Shape == Shape::OuterLeft) return 16732;
					else return 16734;
				else
					if (Shape == Shape::Straight) return 16736;
					else if (Shape == Shape::InnerLeft) return 16738;
					else if (Shape == Shape::InnerRight) return 16740;
					else if (Shape == Shape::OuterLeft) return 16742;
					else return 16744;
		}
		BlockState PolishedBlackstoneStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace PolishedBlackstoneWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState PolishedBlackstoneWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16780;
							else if (West == West::Low) return 16781;
							else return 16782;
						else
							if (West == West::None) return 16786;
							else if (West == West::Low) return 16787;
							else return 16788;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16792;
							else if (West == West::Low) return 16793;
							else return 16794;
						else
							if (West == West::None) return 16798;
							else if (West == West::Low) return 16799;
							else return 16800;
					else
						if (Up)
							if (West == West::None) return 16804;
							else if (West == West::Low) return 16805;
							else return 16806;
						else
							if (West == West::None) return 16810;
							else if (West == West::Low) return 16811;
							else return 16812;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16816;
							else if (West == West::Low) return 16817;
							else return 16818;
						else
							if (West == West::None) return 16822;
							else if (West == West::Low) return 16823;
							else return 16824;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16828;
							else if (West == West::Low) return 16829;
							else return 16830;
						else
							if (West == West::None) return 16834;
							else if (West == West::Low) return 16835;
							else return 16836;
					else
						if (Up)
							if (West == West::None) return 16840;
							else if (West == West::Low) return 16841;
							else return 16842;
						else
							if (West == West::None) return 16846;
							else if (West == West::Low) return 16847;
							else return 16848;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16852;
							else if (West == West::Low) return 16853;
							else return 16854;
						else
							if (West == West::None) return 16858;
							else if (West == West::Low) return 16859;
							else return 16860;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16864;
							else if (West == West::Low) return 16865;
							else return 16866;
						else
							if (West == West::None) return 16870;
							else if (West == West::Low) return 16871;
							else return 16872;
					else
						if (Up)
							if (West == West::None) return 16876;
							else if (West == West::Low) return 16877;
							else return 16878;
						else
							if (West == West::None) return 16882;
							else if (West == West::Low) return 16883;
							else return 16884;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16888;
							else if (West == West::Low) return 16889;
							else return 16890;
						else
							if (West == West::None) return 16894;
							else if (West == West::Low) return 16895;
							else return 16896;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16900;
							else if (West == West::Low) return 16901;
							else return 16902;
						else
							if (West == West::None) return 16906;
							else if (West == West::Low) return 16907;
							else return 16908;
					else
						if (Up)
							if (West == West::None) return 16912;
							else if (West == West::Low) return 16913;
							else return 16914;
						else
							if (West == West::None) return 16918;
							else if (West == West::Low) return 16919;
							else return 16920;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16924;
							else if (West == West::Low) return 16925;
							else return 16926;
						else
							if (West == West::None) return 16930;
							else if (West == West::Low) return 16931;
							else return 16932;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16936;
							else if (West == West::Low) return 16937;
							else return 16938;
						else
							if (West == West::None) return 16942;
							else if (West == West::Low) return 16943;
							else return 16944;
					else
						if (Up)
							if (West == West::None) return 16948;
							else if (West == West::Low) return 16949;
							else return 16950;
						else
							if (West == West::None) return 16954;
							else if (West == West::Low) return 16955;
							else return 16956;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16960;
							else if (West == West::Low) return 16961;
							else return 16962;
						else
							if (West == West::None) return 16966;
							else if (West == West::Low) return 16967;
							else return 16968;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 16972;
							else if (West == West::Low) return 16973;
							else return 16974;
						else
							if (West == West::None) return 16978;
							else if (West == West::Low) return 16979;
							else return 16980;
					else
						if (Up)
							if (West == West::None) return 16984;
							else if (West == West::Low) return 16985;
							else return 16986;
						else
							if (West == West::None) return 16990;
							else if (West == West::Low) return 16991;
							else return 16992;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 16996;
							else if (West == West::Low) return 16997;
							else return 16998;
						else
							if (West == West::None) return 17002;
							else if (West == West::Low) return 17003;
							else return 17004;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 17008;
							else if (West == West::Low) return 17009;
							else return 17010;
						else
							if (West == West::None) return 17014;
							else if (West == West::Low) return 17015;
							else return 17016;
					else
						if (Up)
							if (West == West::None) return 17020;
							else if (West == West::Low) return 17021;
							else return 17022;
						else
							if (West == West::None) return 17026;
							else if (West == West::Low) return 17027;
							else return 17028;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 17032;
							else if (West == West::Low) return 17033;
							else return 17034;
						else
							if (West == West::None) return 17038;
							else if (West == West::Low) return 17039;
							else return 17040;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 17044;
							else if (West == West::Low) return 17045;
							else return 17046;
						else
							if (West == West::None) return 17050;
							else if (West == West::Low) return 17051;
							else return 17052;
					else
						if (Up)
							if (West == West::None) return 17056;
							else if (West == West::Low) return 17057;
							else return 17058;
						else
							if (West == West::None) return 17062;
							else if (West == West::Low) return 17063;
							else return 17064;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 17068;
							else if (West == West::Low) return 17069;
							else return 17070;
						else
							if (West == West::None) return 17074;
							else if (West == West::Low) return 17075;
							else return 17076;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 17080;
							else if (West == West::Low) return 17081;
							else return 17082;
						else
							if (West == West::None) return 17086;
							else if (West == West::Low) return 17087;
							else return 17088;
					else
						if (Up)
							if (West == West::None) return 17092;
							else if (West == West::Low) return 17093;
							else return 17094;
						else
							if (West == West::None) return 17098;
							else if (West == West::Low) return 17099;
							else return 17100;
		}
		BlockState PolishedBlackstoneWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace PolishedDiorite
	{
		constexpr BlockState PolishedDiorite()
		{
			return 5;
		}
	}
	namespace PolishedDioriteSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState PolishedDioriteSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10808;
			else if (Type == Type::Bottom) return 10810;
			else return 10812;
		}
		BlockState PolishedDioriteSlab();
		enum Type Type(BlockState Block);
	}
	namespace PolishedDioriteStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState PolishedDioriteStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9910;
					else if (Shape == Shape::InnerLeft) return 9912;
					else if (Shape == Shape::InnerRight) return 9914;
					else if (Shape == Shape::OuterLeft) return 9916;
					else return 9918;
				else
					if (Shape == Shape::Straight) return 9920;
					else if (Shape == Shape::InnerLeft) return 9922;
					else if (Shape == Shape::InnerRight) return 9924;
					else if (Shape == Shape::OuterLeft) return 9926;
					else return 9928;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9930;
					else if (Shape == Shape::InnerLeft) return 9932;
					else if (Shape == Shape::InnerRight) return 9934;
					else if (Shape == Shape::OuterLeft) return 9936;
					else return 9938;
				else
					if (Shape == Shape::Straight) return 9940;
					else if (Shape == Shape::InnerLeft) return 9942;
					else if (Shape == Shape::InnerRight) return 9944;
					else if (Shape == Shape::OuterLeft) return 9946;
					else return 9948;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9950;
					else if (Shape == Shape::InnerLeft) return 9952;
					else if (Shape == Shape::InnerRight) return 9954;
					else if (Shape == Shape::OuterLeft) return 9956;
					else return 9958;
				else
					if (Shape == Shape::Straight) return 9960;
					else if (Shape == Shape::InnerLeft) return 9962;
					else if (Shape == Shape::InnerRight) return 9964;
					else if (Shape == Shape::OuterLeft) return 9966;
					else return 9968;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9970;
					else if (Shape == Shape::InnerLeft) return 9972;
					else if (Shape == Shape::InnerRight) return 9974;
					else if (Shape == Shape::OuterLeft) return 9976;
					else return 9978;
				else
					if (Shape == Shape::Straight) return 9980;
					else if (Shape == Shape::InnerLeft) return 9982;
					else if (Shape == Shape::InnerRight) return 9984;
					else if (Shape == Shape::OuterLeft) return 9986;
					else return 9988;
		}
		BlockState PolishedDioriteStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace PolishedGranite
	{
		constexpr BlockState PolishedGranite()
		{
			return 3;
		}
	}
	namespace PolishedGraniteSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState PolishedGraniteSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10790;
			else if (Type == Type::Bottom) return 10792;
			else return 10794;
		}
		BlockState PolishedGraniteSlab();
		enum Type Type(BlockState Block);
	}
	namespace PolishedGraniteStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState PolishedGraniteStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9670;
					else if (Shape == Shape::InnerLeft) return 9672;
					else if (Shape == Shape::InnerRight) return 9674;
					else if (Shape == Shape::OuterLeft) return 9676;
					else return 9678;
				else
					if (Shape == Shape::Straight) return 9680;
					else if (Shape == Shape::InnerLeft) return 9682;
					else if (Shape == Shape::InnerRight) return 9684;
					else if (Shape == Shape::OuterLeft) return 9686;
					else return 9688;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9690;
					else if (Shape == Shape::InnerLeft) return 9692;
					else if (Shape == Shape::InnerRight) return 9694;
					else if (Shape == Shape::OuterLeft) return 9696;
					else return 9698;
				else
					if (Shape == Shape::Straight) return 9700;
					else if (Shape == Shape::InnerLeft) return 9702;
					else if (Shape == Shape::InnerRight) return 9704;
					else if (Shape == Shape::OuterLeft) return 9706;
					else return 9708;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9710;
					else if (Shape == Shape::InnerLeft) return 9712;
					else if (Shape == Shape::InnerRight) return 9714;
					else if (Shape == Shape::OuterLeft) return 9716;
					else return 9718;
				else
					if (Shape == Shape::Straight) return 9720;
					else if (Shape == Shape::InnerLeft) return 9722;
					else if (Shape == Shape::InnerRight) return 9724;
					else if (Shape == Shape::OuterLeft) return 9726;
					else return 9728;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9730;
					else if (Shape == Shape::InnerLeft) return 9732;
					else if (Shape == Shape::InnerRight) return 9734;
					else if (Shape == Shape::OuterLeft) return 9736;
					else return 9738;
				else
					if (Shape == Shape::Straight) return 9740;
					else if (Shape == Shape::InnerLeft) return 9742;
					else if (Shape == Shape::InnerRight) return 9744;
					else if (Shape == Shape::OuterLeft) return 9746;
					else return 9748;
		}
		BlockState PolishedGraniteStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace Poppy
	{
		constexpr BlockState Poppy()
		{
			return 1413;
		}
	}
	namespace Potatoes
	{
		constexpr BlockState Potatoes(const unsigned char Age)
		{
			if (Age == 0) return 6338;
			else if (Age == 1) return 6339;
			else if (Age == 2) return 6340;
			else if (Age == 3) return 6341;
			else if (Age == 4) return 6342;
			else if (Age == 5) return 6343;
			else if (Age == 6) return 6344;
			else return 6345;
		}
		BlockState Potatoes();
		unsigned char Age(BlockState Block);
	}
	namespace PottedAcaciaSapling
	{
		constexpr BlockState PottedAcaciaSapling()
		{
			return 6310;
		}
	}
	namespace PottedAllium
	{
		constexpr BlockState PottedAllium()
		{
			return 6316;
		}
	}
	namespace PottedAzureBluet
	{
		constexpr BlockState PottedAzureBluet()
		{
			return 6317;
		}
	}
	namespace PottedBamboo
	{
		constexpr BlockState PottedBamboo()
		{
			return 9664;
		}
	}
	namespace PottedBirchSapling
	{
		constexpr BlockState PottedBirchSapling()
		{
			return 6308;
		}
	}
	namespace PottedBlueOrchid
	{
		constexpr BlockState PottedBlueOrchid()
		{
			return 6315;
		}
	}
	namespace PottedBrownMushroom
	{
		constexpr BlockState PottedBrownMushroom()
		{
			return 6327;
		}
	}
	namespace PottedCactus
	{
		constexpr BlockState PottedCactus()
		{
			return 6329;
		}
	}
	namespace PottedCornflower
	{
		constexpr BlockState PottedCornflower()
		{
			return 6323;
		}
	}
	namespace PottedCrimsonFungus
	{
		constexpr BlockState PottedCrimsonFungus()
		{
			return 15834;
		}
	}
	namespace PottedCrimsonRoots
	{
		constexpr BlockState PottedCrimsonRoots()
		{
			return 15836;
		}
	}
	namespace PottedDandelion
	{
		constexpr BlockState PottedDandelion()
		{
			return 6313;
		}
	}
	namespace PottedDarkOakSapling
	{
		constexpr BlockState PottedDarkOakSapling()
		{
			return 6311;
		}
	}
	namespace PottedDeadBush
	{
		constexpr BlockState PottedDeadBush()
		{
			return 6328;
		}
	}
	namespace PottedFern
	{
		constexpr BlockState PottedFern()
		{
			return 6312;
		}
	}
	namespace PottedJungleSapling
	{
		constexpr BlockState PottedJungleSapling()
		{
			return 6309;
		}
	}
	namespace PottedLilyOfTheValley
	{
		constexpr BlockState PottedLilyOfTheValley()
		{
			return 6324;
		}
	}
	namespace PottedOakSapling
	{
		constexpr BlockState PottedOakSapling()
		{
			return 6306;
		}
	}
	namespace PottedOrangeTulip
	{
		constexpr BlockState PottedOrangeTulip()
		{
			return 6319;
		}
	}
	namespace PottedOxeyeDaisy
	{
		constexpr BlockState PottedOxeyeDaisy()
		{
			return 6322;
		}
	}
	namespace PottedPinkTulip
	{
		constexpr BlockState PottedPinkTulip()
		{
			return 6321;
		}
	}
	namespace PottedPoppy
	{
		constexpr BlockState PottedPoppy()
		{
			return 6314;
		}
	}
	namespace PottedRedMushroom
	{
		constexpr BlockState PottedRedMushroom()
		{
			return 6326;
		}
	}
	namespace PottedRedTulip
	{
		constexpr BlockState PottedRedTulip()
		{
			return 6318;
		}
	}
	namespace PottedSpruceSapling
	{
		constexpr BlockState PottedSpruceSapling()
		{
			return 6307;
		}
	}
	namespace PottedWarpedFungus
	{
		constexpr BlockState PottedWarpedFungus()
		{
			return 15835;
		}
	}
	namespace PottedWarpedRoots
	{
		constexpr BlockState PottedWarpedRoots()
		{
			return 15837;
		}
	}
	namespace PottedWhiteTulip
	{
		constexpr BlockState PottedWhiteTulip()
		{
			return 6320;
		}
	}
	namespace PottedWitherRose
	{
		constexpr BlockState PottedWitherRose()
		{
			return 6325;
		}
	}
	namespace PoweredRail
	{
		enum class Shape
		{
			NorthSouth,
			EastWest,
			AscendingEast,
			AscendingWest,
			AscendingNorth,
			AscendingSouth
		};
		constexpr BlockState PoweredRail(const bool Powered, const enum Shape Shape)
		{
			if (Powered)
				if (Shape == Shape::NorthSouth) return 1305;
				else if (Shape == Shape::EastWest) return 1306;
				else if (Shape == Shape::AscendingEast) return 1307;
				else if (Shape == Shape::AscendingWest) return 1308;
				else if (Shape == Shape::AscendingNorth) return 1309;
				else return 1310;
			else
				if (Shape == Shape::NorthSouth) return 1311;
				else if (Shape == Shape::EastWest) return 1312;
				else if (Shape == Shape::AscendingEast) return 1313;
				else if (Shape == Shape::AscendingWest) return 1314;
				else if (Shape == Shape::AscendingNorth) return 1315;
				else return 1316;
		}
		BlockState PoweredRail();
		bool Powered(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace Prismarine
	{
		constexpr BlockState Prismarine()
		{
			return 7601;
		}
	}
	namespace PrismarineBrickSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState PrismarineBrickSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 7851;
			else if (Type == Type::Bottom) return 7853;
			else return 7855;
		}
		BlockState PrismarineBrickSlab();
		enum Type Type(BlockState Block);
	}
	namespace PrismarineBrickStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState PrismarineBrickStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7685;
					else if (Shape == Shape::InnerLeft) return 7687;
					else if (Shape == Shape::InnerRight) return 7689;
					else if (Shape == Shape::OuterLeft) return 7691;
					else return 7693;
				else
					if (Shape == Shape::Straight) return 7695;
					else if (Shape == Shape::InnerLeft) return 7697;
					else if (Shape == Shape::InnerRight) return 7699;
					else if (Shape == Shape::OuterLeft) return 7701;
					else return 7703;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7705;
					else if (Shape == Shape::InnerLeft) return 7707;
					else if (Shape == Shape::InnerRight) return 7709;
					else if (Shape == Shape::OuterLeft) return 7711;
					else return 7713;
				else
					if (Shape == Shape::Straight) return 7715;
					else if (Shape == Shape::InnerLeft) return 7717;
					else if (Shape == Shape::InnerRight) return 7719;
					else if (Shape == Shape::OuterLeft) return 7721;
					else return 7723;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7725;
					else if (Shape == Shape::InnerLeft) return 7727;
					else if (Shape == Shape::InnerRight) return 7729;
					else if (Shape == Shape::OuterLeft) return 7731;
					else return 7733;
				else
					if (Shape == Shape::Straight) return 7735;
					else if (Shape == Shape::InnerLeft) return 7737;
					else if (Shape == Shape::InnerRight) return 7739;
					else if (Shape == Shape::OuterLeft) return 7741;
					else return 7743;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7745;
					else if (Shape == Shape::InnerLeft) return 7747;
					else if (Shape == Shape::InnerRight) return 7749;
					else if (Shape == Shape::OuterLeft) return 7751;
					else return 7753;
				else
					if (Shape == Shape::Straight) return 7755;
					else if (Shape == Shape::InnerLeft) return 7757;
					else if (Shape == Shape::InnerRight) return 7759;
					else if (Shape == Shape::OuterLeft) return 7761;
					else return 7763;
		}
		BlockState PrismarineBrickStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace PrismarineBricks
	{
		constexpr BlockState PrismarineBricks()
		{
			return 7602;
		}
	}
	namespace PrismarineSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState PrismarineSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 7845;
			else if (Type == Type::Bottom) return 7847;
			else return 7849;
		}
		BlockState PrismarineSlab();
		enum Type Type(BlockState Block);
	}
	namespace PrismarineStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState PrismarineStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7605;
					else if (Shape == Shape::InnerLeft) return 7607;
					else if (Shape == Shape::InnerRight) return 7609;
					else if (Shape == Shape::OuterLeft) return 7611;
					else return 7613;
				else
					if (Shape == Shape::Straight) return 7615;
					else if (Shape == Shape::InnerLeft) return 7617;
					else if (Shape == Shape::InnerRight) return 7619;
					else if (Shape == Shape::OuterLeft) return 7621;
					else return 7623;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7625;
					else if (Shape == Shape::InnerLeft) return 7627;
					else if (Shape == Shape::InnerRight) return 7629;
					else if (Shape == Shape::OuterLeft) return 7631;
					else return 7633;
				else
					if (Shape == Shape::Straight) return 7635;
					else if (Shape == Shape::InnerLeft) return 7637;
					else if (Shape == Shape::InnerRight) return 7639;
					else if (Shape == Shape::OuterLeft) return 7641;
					else return 7643;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7645;
					else if (Shape == Shape::InnerLeft) return 7647;
					else if (Shape == Shape::InnerRight) return 7649;
					else if (Shape == Shape::OuterLeft) return 7651;
					else return 7653;
				else
					if (Shape == Shape::Straight) return 7655;
					else if (Shape == Shape::InnerLeft) return 7657;
					else if (Shape == Shape::InnerRight) return 7659;
					else if (Shape == Shape::OuterLeft) return 7661;
					else return 7663;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 7665;
					else if (Shape == Shape::InnerLeft) return 7667;
					else if (Shape == Shape::InnerRight) return 7669;
					else if (Shape == Shape::OuterLeft) return 7671;
					else return 7673;
				else
					if (Shape == Shape::Straight) return 7675;
					else if (Shape == Shape::InnerLeft) return 7677;
					else if (Shape == Shape::InnerRight) return 7679;
					else if (Shape == Shape::OuterLeft) return 7681;
					else return 7683;
		}
		BlockState PrismarineStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace PrismarineWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState PrismarineWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11194;
							else if (West == West::Low) return 11195;
							else return 11196;
						else
							if (West == West::None) return 11200;
							else if (West == West::Low) return 11201;
							else return 11202;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11206;
							else if (West == West::Low) return 11207;
							else return 11208;
						else
							if (West == West::None) return 11212;
							else if (West == West::Low) return 11213;
							else return 11214;
					else
						if (Up)
							if (West == West::None) return 11218;
							else if (West == West::Low) return 11219;
							else return 11220;
						else
							if (West == West::None) return 11224;
							else if (West == West::Low) return 11225;
							else return 11226;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11230;
							else if (West == West::Low) return 11231;
							else return 11232;
						else
							if (West == West::None) return 11236;
							else if (West == West::Low) return 11237;
							else return 11238;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11242;
							else if (West == West::Low) return 11243;
							else return 11244;
						else
							if (West == West::None) return 11248;
							else if (West == West::Low) return 11249;
							else return 11250;
					else
						if (Up)
							if (West == West::None) return 11254;
							else if (West == West::Low) return 11255;
							else return 11256;
						else
							if (West == West::None) return 11260;
							else if (West == West::Low) return 11261;
							else return 11262;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11266;
							else if (West == West::Low) return 11267;
							else return 11268;
						else
							if (West == West::None) return 11272;
							else if (West == West::Low) return 11273;
							else return 11274;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11278;
							else if (West == West::Low) return 11279;
							else return 11280;
						else
							if (West == West::None) return 11284;
							else if (West == West::Low) return 11285;
							else return 11286;
					else
						if (Up)
							if (West == West::None) return 11290;
							else if (West == West::Low) return 11291;
							else return 11292;
						else
							if (West == West::None) return 11296;
							else if (West == West::Low) return 11297;
							else return 11298;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11302;
							else if (West == West::Low) return 11303;
							else return 11304;
						else
							if (West == West::None) return 11308;
							else if (West == West::Low) return 11309;
							else return 11310;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11314;
							else if (West == West::Low) return 11315;
							else return 11316;
						else
							if (West == West::None) return 11320;
							else if (West == West::Low) return 11321;
							else return 11322;
					else
						if (Up)
							if (West == West::None) return 11326;
							else if (West == West::Low) return 11327;
							else return 11328;
						else
							if (West == West::None) return 11332;
							else if (West == West::Low) return 11333;
							else return 11334;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11338;
							else if (West == West::Low) return 11339;
							else return 11340;
						else
							if (West == West::None) return 11344;
							else if (West == West::Low) return 11345;
							else return 11346;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11350;
							else if (West == West::Low) return 11351;
							else return 11352;
						else
							if (West == West::None) return 11356;
							else if (West == West::Low) return 11357;
							else return 11358;
					else
						if (Up)
							if (West == West::None) return 11362;
							else if (West == West::Low) return 11363;
							else return 11364;
						else
							if (West == West::None) return 11368;
							else if (West == West::Low) return 11369;
							else return 11370;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11374;
							else if (West == West::Low) return 11375;
							else return 11376;
						else
							if (West == West::None) return 11380;
							else if (West == West::Low) return 11381;
							else return 11382;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11386;
							else if (West == West::Low) return 11387;
							else return 11388;
						else
							if (West == West::None) return 11392;
							else if (West == West::Low) return 11393;
							else return 11394;
					else
						if (Up)
							if (West == West::None) return 11398;
							else if (West == West::Low) return 11399;
							else return 11400;
						else
							if (West == West::None) return 11404;
							else if (West == West::Low) return 11405;
							else return 11406;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11410;
							else if (West == West::Low) return 11411;
							else return 11412;
						else
							if (West == West::None) return 11416;
							else if (West == West::Low) return 11417;
							else return 11418;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11422;
							else if (West == West::Low) return 11423;
							else return 11424;
						else
							if (West == West::None) return 11428;
							else if (West == West::Low) return 11429;
							else return 11430;
					else
						if (Up)
							if (West == West::None) return 11434;
							else if (West == West::Low) return 11435;
							else return 11436;
						else
							if (West == West::None) return 11440;
							else if (West == West::Low) return 11441;
							else return 11442;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11446;
							else if (West == West::Low) return 11447;
							else return 11448;
						else
							if (West == West::None) return 11452;
							else if (West == West::Low) return 11453;
							else return 11454;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11458;
							else if (West == West::Low) return 11459;
							else return 11460;
						else
							if (West == West::None) return 11464;
							else if (West == West::Low) return 11465;
							else return 11466;
					else
						if (Up)
							if (West == West::None) return 11470;
							else if (West == West::Low) return 11471;
							else return 11472;
						else
							if (West == West::None) return 11476;
							else if (West == West::Low) return 11477;
							else return 11478;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11482;
							else if (West == West::Low) return 11483;
							else return 11484;
						else
							if (West == West::None) return 11488;
							else if (West == West::Low) return 11489;
							else return 11490;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11494;
							else if (West == West::Low) return 11495;
							else return 11496;
						else
							if (West == West::None) return 11500;
							else if (West == West::Low) return 11501;
							else return 11502;
					else
						if (Up)
							if (West == West::None) return 11506;
							else if (West == West::Low) return 11507;
							else return 11508;
						else
							if (West == West::None) return 11512;
							else if (West == West::Low) return 11513;
							else return 11514;
		}
		BlockState PrismarineWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace Pumpkin
	{
		constexpr BlockState Pumpkin()
		{
			return 3998;
		}
	}
	namespace PumpkinStem
	{
		constexpr BlockState PumpkinStem(const unsigned char Age)
		{
			if (Age == 0) return 4772;
			else if (Age == 1) return 4773;
			else if (Age == 2) return 4774;
			else if (Age == 3) return 4775;
			else if (Age == 4) return 4776;
			else if (Age == 5) return 4777;
			else if (Age == 6) return 4778;
			else return 4779;
		}
		BlockState PumpkinStem();
		unsigned char Age(BlockState Block);
	}
	namespace PurpleBanner
	{
		constexpr BlockState PurpleBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 8057;
			else if (Rotation == 1) return 8058;
			else if (Rotation == 2) return 8059;
			else if (Rotation == 3) return 8060;
			else if (Rotation == 4) return 8061;
			else if (Rotation == 5) return 8062;
			else if (Rotation == 6) return 8063;
			else if (Rotation == 7) return 8064;
			else if (Rotation == 8) return 8065;
			else if (Rotation == 9) return 8066;
			else if (Rotation == 10) return 8067;
			else if (Rotation == 11) return 8068;
			else if (Rotation == 12) return 8069;
			else if (Rotation == 13) return 8070;
			else if (Rotation == 14) return 8071;
			else return 8072;
		}
		BlockState PurpleBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace PurpleBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState PurpleBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1209;
					else return 1210;
				else
					if (Part == Part::Head) return 1211;
					else return 1212;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1213;
					else return 1214;
				else
					if (Part == Part::Head) return 1215;
					else return 1216;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1217;
					else return 1218;
				else
					if (Part == Part::Head) return 1219;
					else return 1220;
			else
				if (Occupied)
					if (Part == Part::Head) return 1221;
					else return 1222;
				else
					if (Part == Part::Head) return 1223;
					else return 1224;
		}
		BlockState PurpleBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace PurpleCarpet
	{
		constexpr BlockState PurpleCarpet()
		{
			return 7876;
		}
	}
	namespace PurpleConcrete
	{
		constexpr BlockState PurpleConcrete()
		{
			return 9448;
		}
	}
	namespace PurpleConcretePowder
	{
		constexpr BlockState PurpleConcretePowder()
		{
			return 9464;
		}
	}
	namespace PurpleGlazedTerracotta
	{
		constexpr BlockState PurpleGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9414;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9415;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9416;
			else return 9417;
		}
		BlockState PurpleGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace PurpleShulkerBox
	{
		constexpr BlockState PurpleShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9338;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9339;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9340;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9341;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9342;
			else return 9343;
		}
		BlockState PurpleShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace PurpleStainedGlass
	{
		constexpr BlockState PurpleStainedGlass()
		{
			return 4105;
		}
	}
	namespace PurpleStainedGlassPane
	{
		constexpr BlockState PurpleStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7185;
						else return 7186;
					else
						if (West) return 7189;
						else return 7190;
				else
					if (South)
						if (West) return 7193;
						else return 7194;
					else
						if (West) return 7197;
						else return 7198;
			else
				if (North)
					if (South)
						if (West) return 7201;
						else return 7202;
					else
						if (West) return 7205;
						else return 7206;
				else
					if (South)
						if (West) return 7209;
						else return 7210;
					else
						if (West) return 7213;
						else return 7214;
		}
		BlockState PurpleStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace PurpleTerracotta
	{
		constexpr BlockState PurpleTerracotta()
		{
			return 6857;
		}
	}
	namespace PurpleWallBanner
	{
		constexpr BlockState PurpleWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8193;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8194;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8195;
			else return 8196;
		}
		BlockState PurpleWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace PurpleWool
	{
		constexpr BlockState PurpleWool()
		{
			return 1394;
		}
	}
	namespace PurpurBlock
	{
		constexpr BlockState PurpurBlock()
		{
			return 9134;
		}
	}
	namespace PurpurPillar
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState PurpurPillar(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 9135;
			else if (Axis == Axis::Y) return 9136;
			else return 9137;
		}
		BlockState PurpurPillar();
		enum Axis Axis(BlockState Block);
	}
	namespace PurpurSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState PurpurSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8409;
			else if (Type == Type::Bottom) return 8411;
			else return 8413;
		}
		BlockState PurpurSlab();
		enum Type Type(BlockState Block);
	}
	namespace PurpurStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState PurpurStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9139;
					else if (Shape == Shape::InnerLeft) return 9141;
					else if (Shape == Shape::InnerRight) return 9143;
					else if (Shape == Shape::OuterLeft) return 9145;
					else return 9147;
				else
					if (Shape == Shape::Straight) return 9149;
					else if (Shape == Shape::InnerLeft) return 9151;
					else if (Shape == Shape::InnerRight) return 9153;
					else if (Shape == Shape::OuterLeft) return 9155;
					else return 9157;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9159;
					else if (Shape == Shape::InnerLeft) return 9161;
					else if (Shape == Shape::InnerRight) return 9163;
					else if (Shape == Shape::OuterLeft) return 9165;
					else return 9167;
				else
					if (Shape == Shape::Straight) return 9169;
					else if (Shape == Shape::InnerLeft) return 9171;
					else if (Shape == Shape::InnerRight) return 9173;
					else if (Shape == Shape::OuterLeft) return 9175;
					else return 9177;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9179;
					else if (Shape == Shape::InnerLeft) return 9181;
					else if (Shape == Shape::InnerRight) return 9183;
					else if (Shape == Shape::OuterLeft) return 9185;
					else return 9187;
				else
					if (Shape == Shape::Straight) return 9189;
					else if (Shape == Shape::InnerLeft) return 9191;
					else if (Shape == Shape::InnerRight) return 9193;
					else if (Shape == Shape::OuterLeft) return 9195;
					else return 9197;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9199;
					else if (Shape == Shape::InnerLeft) return 9201;
					else if (Shape == Shape::InnerRight) return 9203;
					else if (Shape == Shape::OuterLeft) return 9205;
					else return 9207;
				else
					if (Shape == Shape::Straight) return 9209;
					else if (Shape == Shape::InnerLeft) return 9211;
					else if (Shape == Shape::InnerRight) return 9213;
					else if (Shape == Shape::OuterLeft) return 9215;
					else return 9217;
		}
		BlockState PurpurStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace QuartzBlock
	{
		constexpr BlockState QuartzBlock()
		{
			return 6738;
		}
	}
	namespace QuartzBricks
	{
		constexpr BlockState QuartzBricks()
		{
			return 17103;
		}
	}
	namespace QuartzPillar
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState QuartzPillar(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 6740;
			else if (Axis == Axis::Y) return 6741;
			else return 6742;
		}
		BlockState QuartzPillar();
		enum Axis Axis(BlockState Block);
	}
	namespace QuartzSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState QuartzSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8391;
			else if (Type == Type::Bottom) return 8393;
			else return 8395;
		}
		BlockState QuartzSlab();
		enum Type Type(BlockState Block);
	}
	namespace QuartzStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState QuartzStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 6744;
					else if (Shape == Shape::InnerLeft) return 6746;
					else if (Shape == Shape::InnerRight) return 6748;
					else if (Shape == Shape::OuterLeft) return 6750;
					else return 6752;
				else
					if (Shape == Shape::Straight) return 6754;
					else if (Shape == Shape::InnerLeft) return 6756;
					else if (Shape == Shape::InnerRight) return 6758;
					else if (Shape == Shape::OuterLeft) return 6760;
					else return 6762;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 6764;
					else if (Shape == Shape::InnerLeft) return 6766;
					else if (Shape == Shape::InnerRight) return 6768;
					else if (Shape == Shape::OuterLeft) return 6770;
					else return 6772;
				else
					if (Shape == Shape::Straight) return 6774;
					else if (Shape == Shape::InnerLeft) return 6776;
					else if (Shape == Shape::InnerRight) return 6778;
					else if (Shape == Shape::OuterLeft) return 6780;
					else return 6782;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 6784;
					else if (Shape == Shape::InnerLeft) return 6786;
					else if (Shape == Shape::InnerRight) return 6788;
					else if (Shape == Shape::OuterLeft) return 6790;
					else return 6792;
				else
					if (Shape == Shape::Straight) return 6794;
					else if (Shape == Shape::InnerLeft) return 6796;
					else if (Shape == Shape::InnerRight) return 6798;
					else if (Shape == Shape::OuterLeft) return 6800;
					else return 6802;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 6804;
					else if (Shape == Shape::InnerLeft) return 6806;
					else if (Shape == Shape::InnerRight) return 6808;
					else if (Shape == Shape::OuterLeft) return 6810;
					else return 6812;
				else
					if (Shape == Shape::Straight) return 6814;
					else if (Shape == Shape::InnerLeft) return 6816;
					else if (Shape == Shape::InnerRight) return 6818;
					else if (Shape == Shape::OuterLeft) return 6820;
					else return 6822;
		}
		BlockState QuartzStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace Rail
	{
		enum class Shape
		{
			NorthSouth,
			EastWest,
			AscendingEast,
			AscendingWest,
			AscendingNorth,
			AscendingSouth,
			SouthEast,
			SouthWest,
			NorthWest,
			NorthEast
		};
		constexpr BlockState Rail(const enum Shape Shape)
		{
			if (Shape == Shape::NorthSouth) return 3645;
			else if (Shape == Shape::EastWest) return 3646;
			else if (Shape == Shape::AscendingEast) return 3647;
			else if (Shape == Shape::AscendingWest) return 3648;
			else if (Shape == Shape::AscendingNorth) return 3649;
			else if (Shape == Shape::AscendingSouth) return 3650;
			else if (Shape == Shape::SouthEast) return 3651;
			else if (Shape == Shape::SouthWest) return 3652;
			else if (Shape == Shape::NorthWest) return 3653;
			else return 3654;
		}
		BlockState Rail();
		enum Shape Shape(BlockState Block);
	}
	namespace RedBanner
	{
		constexpr BlockState RedBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 8121;
			else if (Rotation == 1) return 8122;
			else if (Rotation == 2) return 8123;
			else if (Rotation == 3) return 8124;
			else if (Rotation == 4) return 8125;
			else if (Rotation == 5) return 8126;
			else if (Rotation == 6) return 8127;
			else if (Rotation == 7) return 8128;
			else if (Rotation == 8) return 8129;
			else if (Rotation == 9) return 8130;
			else if (Rotation == 10) return 8131;
			else if (Rotation == 11) return 8132;
			else if (Rotation == 12) return 8133;
			else if (Rotation == 13) return 8134;
			else if (Rotation == 14) return 8135;
			else return 8136;
		}
		BlockState RedBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace RedBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState RedBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1273;
					else return 1274;
				else
					if (Part == Part::Head) return 1275;
					else return 1276;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1277;
					else return 1278;
				else
					if (Part == Part::Head) return 1279;
					else return 1280;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1281;
					else return 1282;
				else
					if (Part == Part::Head) return 1283;
					else return 1284;
			else
				if (Occupied)
					if (Part == Part::Head) return 1285;
					else return 1286;
				else
					if (Part == Part::Head) return 1287;
					else return 1288;
		}
		BlockState RedBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace RedCarpet
	{
		constexpr BlockState RedCarpet()
		{
			return 7880;
		}
	}
	namespace RedConcrete
	{
		constexpr BlockState RedConcrete()
		{
			return 9452;
		}
	}
	namespace RedConcretePowder
	{
		constexpr BlockState RedConcretePowder()
		{
			return 9468;
		}
	}
	namespace RedGlazedTerracotta
	{
		constexpr BlockState RedGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9430;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9431;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9432;
			else return 9433;
		}
		BlockState RedGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace RedMushroom
	{
		constexpr BlockState RedMushroom()
		{
			return 1426;
		}
	}
	namespace RedMushroomBlock
	{
		constexpr BlockState RedMushroomBlock(const bool Down, const bool East, const bool North, const bool South, const bool Up, const bool West)
		{
			if (Down)
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 4569;
								else return 4570;
							else
								if (West) return 4571;
								else return 4572;
						else
							if (Up)
								if (West) return 4573;
								else return 4574;
							else
								if (West) return 4575;
								else return 4576;
					else
						if (South)
							if (Up)
								if (West) return 4577;
								else return 4578;
							else
								if (West) return 4579;
								else return 4580;
						else
							if (Up)
								if (West) return 4581;
								else return 4582;
							else
								if (West) return 4583;
								else return 4584;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 4585;
								else return 4586;
							else
								if (West) return 4587;
								else return 4588;
						else
							if (Up)
								if (West) return 4589;
								else return 4590;
							else
								if (West) return 4591;
								else return 4592;
					else
						if (South)
							if (Up)
								if (West) return 4593;
								else return 4594;
							else
								if (West) return 4595;
								else return 4596;
						else
							if (Up)
								if (West) return 4597;
								else return 4598;
							else
								if (West) return 4599;
								else return 4600;
			else
				if (East)
					if (North)
						if (South)
							if (Up)
								if (West) return 4601;
								else return 4602;
							else
								if (West) return 4603;
								else return 4604;
						else
							if (Up)
								if (West) return 4605;
								else return 4606;
							else
								if (West) return 4607;
								else return 4608;
					else
						if (South)
							if (Up)
								if (West) return 4609;
								else return 4610;
							else
								if (West) return 4611;
								else return 4612;
						else
							if (Up)
								if (West) return 4613;
								else return 4614;
							else
								if (West) return 4615;
								else return 4616;
				else
					if (North)
						if (South)
							if (Up)
								if (West) return 4617;
								else return 4618;
							else
								if (West) return 4619;
								else return 4620;
						else
							if (Up)
								if (West) return 4621;
								else return 4622;
							else
								if (West) return 4623;
								else return 4624;
					else
						if (South)
							if (Up)
								if (West) return 4625;
								else return 4626;
							else
								if (West) return 4627;
								else return 4628;
						else
							if (Up)
								if (West) return 4629;
								else return 4630;
							else
								if (West) return 4631;
								else return 4632;
		}
		BlockState RedMushroomBlock();
		bool Down(BlockState Block);
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool Up(BlockState Block);
		bool West(BlockState Block);
	}
	namespace RedNetherBrickSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState RedNetherBrickSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10850;
			else if (Type == Type::Bottom) return 10852;
			else return 10854;
		}
		BlockState RedNetherBrickSlab();
		enum Type Type(BlockState Block);
	}
	namespace RedNetherBrickStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState RedNetherBrickStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10550;
					else if (Shape == Shape::InnerLeft) return 10552;
					else if (Shape == Shape::InnerRight) return 10554;
					else if (Shape == Shape::OuterLeft) return 10556;
					else return 10558;
				else
					if (Shape == Shape::Straight) return 10560;
					else if (Shape == Shape::InnerLeft) return 10562;
					else if (Shape == Shape::InnerRight) return 10564;
					else if (Shape == Shape::OuterLeft) return 10566;
					else return 10568;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10570;
					else if (Shape == Shape::InnerLeft) return 10572;
					else if (Shape == Shape::InnerRight) return 10574;
					else if (Shape == Shape::OuterLeft) return 10576;
					else return 10578;
				else
					if (Shape == Shape::Straight) return 10580;
					else if (Shape == Shape::InnerLeft) return 10582;
					else if (Shape == Shape::InnerRight) return 10584;
					else if (Shape == Shape::OuterLeft) return 10586;
					else return 10588;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10590;
					else if (Shape == Shape::InnerLeft) return 10592;
					else if (Shape == Shape::InnerRight) return 10594;
					else if (Shape == Shape::OuterLeft) return 10596;
					else return 10598;
				else
					if (Shape == Shape::Straight) return 10600;
					else if (Shape == Shape::InnerLeft) return 10602;
					else if (Shape == Shape::InnerRight) return 10604;
					else if (Shape == Shape::OuterLeft) return 10606;
					else return 10608;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10610;
					else if (Shape == Shape::InnerLeft) return 10612;
					else if (Shape == Shape::InnerRight) return 10614;
					else if (Shape == Shape::OuterLeft) return 10616;
					else return 10618;
				else
					if (Shape == Shape::Straight) return 10620;
					else if (Shape == Shape::InnerLeft) return 10622;
					else if (Shape == Shape::InnerRight) return 10624;
					else if (Shape == Shape::OuterLeft) return 10626;
					else return 10628;
		}
		BlockState RedNetherBrickStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace RedNetherBrickWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState RedNetherBrickWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13462;
							else if (West == West::Low) return 13463;
							else return 13464;
						else
							if (West == West::None) return 13468;
							else if (West == West::Low) return 13469;
							else return 13470;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13474;
							else if (West == West::Low) return 13475;
							else return 13476;
						else
							if (West == West::None) return 13480;
							else if (West == West::Low) return 13481;
							else return 13482;
					else
						if (Up)
							if (West == West::None) return 13486;
							else if (West == West::Low) return 13487;
							else return 13488;
						else
							if (West == West::None) return 13492;
							else if (West == West::Low) return 13493;
							else return 13494;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13498;
							else if (West == West::Low) return 13499;
							else return 13500;
						else
							if (West == West::None) return 13504;
							else if (West == West::Low) return 13505;
							else return 13506;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13510;
							else if (West == West::Low) return 13511;
							else return 13512;
						else
							if (West == West::None) return 13516;
							else if (West == West::Low) return 13517;
							else return 13518;
					else
						if (Up)
							if (West == West::None) return 13522;
							else if (West == West::Low) return 13523;
							else return 13524;
						else
							if (West == West::None) return 13528;
							else if (West == West::Low) return 13529;
							else return 13530;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13534;
							else if (West == West::Low) return 13535;
							else return 13536;
						else
							if (West == West::None) return 13540;
							else if (West == West::Low) return 13541;
							else return 13542;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13546;
							else if (West == West::Low) return 13547;
							else return 13548;
						else
							if (West == West::None) return 13552;
							else if (West == West::Low) return 13553;
							else return 13554;
					else
						if (Up)
							if (West == West::None) return 13558;
							else if (West == West::Low) return 13559;
							else return 13560;
						else
							if (West == West::None) return 13564;
							else if (West == West::Low) return 13565;
							else return 13566;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13570;
							else if (West == West::Low) return 13571;
							else return 13572;
						else
							if (West == West::None) return 13576;
							else if (West == West::Low) return 13577;
							else return 13578;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13582;
							else if (West == West::Low) return 13583;
							else return 13584;
						else
							if (West == West::None) return 13588;
							else if (West == West::Low) return 13589;
							else return 13590;
					else
						if (Up)
							if (West == West::None) return 13594;
							else if (West == West::Low) return 13595;
							else return 13596;
						else
							if (West == West::None) return 13600;
							else if (West == West::Low) return 13601;
							else return 13602;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13606;
							else if (West == West::Low) return 13607;
							else return 13608;
						else
							if (West == West::None) return 13612;
							else if (West == West::Low) return 13613;
							else return 13614;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13618;
							else if (West == West::Low) return 13619;
							else return 13620;
						else
							if (West == West::None) return 13624;
							else if (West == West::Low) return 13625;
							else return 13626;
					else
						if (Up)
							if (West == West::None) return 13630;
							else if (West == West::Low) return 13631;
							else return 13632;
						else
							if (West == West::None) return 13636;
							else if (West == West::Low) return 13637;
							else return 13638;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13642;
							else if (West == West::Low) return 13643;
							else return 13644;
						else
							if (West == West::None) return 13648;
							else if (West == West::Low) return 13649;
							else return 13650;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13654;
							else if (West == West::Low) return 13655;
							else return 13656;
						else
							if (West == West::None) return 13660;
							else if (West == West::Low) return 13661;
							else return 13662;
					else
						if (Up)
							if (West == West::None) return 13666;
							else if (West == West::Low) return 13667;
							else return 13668;
						else
							if (West == West::None) return 13672;
							else if (West == West::Low) return 13673;
							else return 13674;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13678;
							else if (West == West::Low) return 13679;
							else return 13680;
						else
							if (West == West::None) return 13684;
							else if (West == West::Low) return 13685;
							else return 13686;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13690;
							else if (West == West::Low) return 13691;
							else return 13692;
						else
							if (West == West::None) return 13696;
							else if (West == West::Low) return 13697;
							else return 13698;
					else
						if (Up)
							if (West == West::None) return 13702;
							else if (West == West::Low) return 13703;
							else return 13704;
						else
							if (West == West::None) return 13708;
							else if (West == West::Low) return 13709;
							else return 13710;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13714;
							else if (West == West::Low) return 13715;
							else return 13716;
						else
							if (West == West::None) return 13720;
							else if (West == West::Low) return 13721;
							else return 13722;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13726;
							else if (West == West::Low) return 13727;
							else return 13728;
						else
							if (West == West::None) return 13732;
							else if (West == West::Low) return 13733;
							else return 13734;
					else
						if (Up)
							if (West == West::None) return 13738;
							else if (West == West::Low) return 13739;
							else return 13740;
						else
							if (West == West::None) return 13744;
							else if (West == West::Low) return 13745;
							else return 13746;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13750;
							else if (West == West::Low) return 13751;
							else return 13752;
						else
							if (West == West::None) return 13756;
							else if (West == West::Low) return 13757;
							else return 13758;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13762;
							else if (West == West::Low) return 13763;
							else return 13764;
						else
							if (West == West::None) return 13768;
							else if (West == West::Low) return 13769;
							else return 13770;
					else
						if (Up)
							if (West == West::None) return 13774;
							else if (West == West::Low) return 13775;
							else return 13776;
						else
							if (West == West::None) return 13780;
							else if (West == West::Low) return 13781;
							else return 13782;
		}
		BlockState RedNetherBrickWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace RedNetherBricks
	{
		constexpr BlockState RedNetherBricks()
		{
			return 9255;
		}
	}
	namespace RedSand
	{
		constexpr BlockState RedSand()
		{
			return 67;
		}
	}
	namespace RedSandstone
	{
		constexpr BlockState RedSandstone()
		{
			return 8217;
		}
	}
	namespace RedSandstoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState RedSandstoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8397;
			else if (Type == Type::Bottom) return 8399;
			else return 8401;
		}
		BlockState RedSandstoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace RedSandstoneStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState RedSandstoneStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 8221;
					else if (Shape == Shape::InnerLeft) return 8223;
					else if (Shape == Shape::InnerRight) return 8225;
					else if (Shape == Shape::OuterLeft) return 8227;
					else return 8229;
				else
					if (Shape == Shape::Straight) return 8231;
					else if (Shape == Shape::InnerLeft) return 8233;
					else if (Shape == Shape::InnerRight) return 8235;
					else if (Shape == Shape::OuterLeft) return 8237;
					else return 8239;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 8241;
					else if (Shape == Shape::InnerLeft) return 8243;
					else if (Shape == Shape::InnerRight) return 8245;
					else if (Shape == Shape::OuterLeft) return 8247;
					else return 8249;
				else
					if (Shape == Shape::Straight) return 8251;
					else if (Shape == Shape::InnerLeft) return 8253;
					else if (Shape == Shape::InnerRight) return 8255;
					else if (Shape == Shape::OuterLeft) return 8257;
					else return 8259;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 8261;
					else if (Shape == Shape::InnerLeft) return 8263;
					else if (Shape == Shape::InnerRight) return 8265;
					else if (Shape == Shape::OuterLeft) return 8267;
					else return 8269;
				else
					if (Shape == Shape::Straight) return 8271;
					else if (Shape == Shape::InnerLeft) return 8273;
					else if (Shape == Shape::InnerRight) return 8275;
					else if (Shape == Shape::OuterLeft) return 8277;
					else return 8279;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 8281;
					else if (Shape == Shape::InnerLeft) return 8283;
					else if (Shape == Shape::InnerRight) return 8285;
					else if (Shape == Shape::OuterLeft) return 8287;
					else return 8289;
				else
					if (Shape == Shape::Straight) return 8291;
					else if (Shape == Shape::InnerLeft) return 8293;
					else if (Shape == Shape::InnerRight) return 8295;
					else if (Shape == Shape::OuterLeft) return 8297;
					else return 8299;
		}
		BlockState RedSandstoneStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace RedSandstoneWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState RedSandstoneWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11518;
							else if (West == West::Low) return 11519;
							else return 11520;
						else
							if (West == West::None) return 11524;
							else if (West == West::Low) return 11525;
							else return 11526;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11530;
							else if (West == West::Low) return 11531;
							else return 11532;
						else
							if (West == West::None) return 11536;
							else if (West == West::Low) return 11537;
							else return 11538;
					else
						if (Up)
							if (West == West::None) return 11542;
							else if (West == West::Low) return 11543;
							else return 11544;
						else
							if (West == West::None) return 11548;
							else if (West == West::Low) return 11549;
							else return 11550;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11554;
							else if (West == West::Low) return 11555;
							else return 11556;
						else
							if (West == West::None) return 11560;
							else if (West == West::Low) return 11561;
							else return 11562;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11566;
							else if (West == West::Low) return 11567;
							else return 11568;
						else
							if (West == West::None) return 11572;
							else if (West == West::Low) return 11573;
							else return 11574;
					else
						if (Up)
							if (West == West::None) return 11578;
							else if (West == West::Low) return 11579;
							else return 11580;
						else
							if (West == West::None) return 11584;
							else if (West == West::Low) return 11585;
							else return 11586;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11590;
							else if (West == West::Low) return 11591;
							else return 11592;
						else
							if (West == West::None) return 11596;
							else if (West == West::Low) return 11597;
							else return 11598;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11602;
							else if (West == West::Low) return 11603;
							else return 11604;
						else
							if (West == West::None) return 11608;
							else if (West == West::Low) return 11609;
							else return 11610;
					else
						if (Up)
							if (West == West::None) return 11614;
							else if (West == West::Low) return 11615;
							else return 11616;
						else
							if (West == West::None) return 11620;
							else if (West == West::Low) return 11621;
							else return 11622;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11626;
							else if (West == West::Low) return 11627;
							else return 11628;
						else
							if (West == West::None) return 11632;
							else if (West == West::Low) return 11633;
							else return 11634;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11638;
							else if (West == West::Low) return 11639;
							else return 11640;
						else
							if (West == West::None) return 11644;
							else if (West == West::Low) return 11645;
							else return 11646;
					else
						if (Up)
							if (West == West::None) return 11650;
							else if (West == West::Low) return 11651;
							else return 11652;
						else
							if (West == West::None) return 11656;
							else if (West == West::Low) return 11657;
							else return 11658;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11662;
							else if (West == West::Low) return 11663;
							else return 11664;
						else
							if (West == West::None) return 11668;
							else if (West == West::Low) return 11669;
							else return 11670;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11674;
							else if (West == West::Low) return 11675;
							else return 11676;
						else
							if (West == West::None) return 11680;
							else if (West == West::Low) return 11681;
							else return 11682;
					else
						if (Up)
							if (West == West::None) return 11686;
							else if (West == West::Low) return 11687;
							else return 11688;
						else
							if (West == West::None) return 11692;
							else if (West == West::Low) return 11693;
							else return 11694;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11698;
							else if (West == West::Low) return 11699;
							else return 11700;
						else
							if (West == West::None) return 11704;
							else if (West == West::Low) return 11705;
							else return 11706;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11710;
							else if (West == West::Low) return 11711;
							else return 11712;
						else
							if (West == West::None) return 11716;
							else if (West == West::Low) return 11717;
							else return 11718;
					else
						if (Up)
							if (West == West::None) return 11722;
							else if (West == West::Low) return 11723;
							else return 11724;
						else
							if (West == West::None) return 11728;
							else if (West == West::Low) return 11729;
							else return 11730;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11734;
							else if (West == West::Low) return 11735;
							else return 11736;
						else
							if (West == West::None) return 11740;
							else if (West == West::Low) return 11741;
							else return 11742;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11746;
							else if (West == West::Low) return 11747;
							else return 11748;
						else
							if (West == West::None) return 11752;
							else if (West == West::Low) return 11753;
							else return 11754;
					else
						if (Up)
							if (West == West::None) return 11758;
							else if (West == West::Low) return 11759;
							else return 11760;
						else
							if (West == West::None) return 11764;
							else if (West == West::Low) return 11765;
							else return 11766;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11770;
							else if (West == West::Low) return 11771;
							else return 11772;
						else
							if (West == West::None) return 11776;
							else if (West == West::Low) return 11777;
							else return 11778;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11782;
							else if (West == West::Low) return 11783;
							else return 11784;
						else
							if (West == West::None) return 11788;
							else if (West == West::Low) return 11789;
							else return 11790;
					else
						if (Up)
							if (West == West::None) return 11794;
							else if (West == West::Low) return 11795;
							else return 11796;
						else
							if (West == West::None) return 11800;
							else if (West == West::Low) return 11801;
							else return 11802;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 11806;
							else if (West == West::Low) return 11807;
							else return 11808;
						else
							if (West == West::None) return 11812;
							else if (West == West::Low) return 11813;
							else return 11814;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 11818;
							else if (West == West::Low) return 11819;
							else return 11820;
						else
							if (West == West::None) return 11824;
							else if (West == West::Low) return 11825;
							else return 11826;
					else
						if (Up)
							if (West == West::None) return 11830;
							else if (West == West::Low) return 11831;
							else return 11832;
						else
							if (West == West::None) return 11836;
							else if (West == West::Low) return 11837;
							else return 11838;
		}
		BlockState RedSandstoneWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace RedShulkerBox
	{
		constexpr BlockState RedShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9362;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9363;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9364;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9365;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9366;
			else return 9367;
		}
		BlockState RedShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace RedStainedGlass
	{
		constexpr BlockState RedStainedGlass()
		{
			return 4109;
		}
	}
	namespace RedStainedGlassPane
	{
		constexpr BlockState RedStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 7313;
						else return 7314;
					else
						if (West) return 7317;
						else return 7318;
				else
					if (South)
						if (West) return 7321;
						else return 7322;
					else
						if (West) return 7325;
						else return 7326;
			else
				if (North)
					if (South)
						if (West) return 7329;
						else return 7330;
					else
						if (West) return 7333;
						else return 7334;
				else
					if (South)
						if (West) return 7337;
						else return 7338;
					else
						if (West) return 7341;
						else return 7342;
		}
		BlockState RedStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace RedTerracotta
	{
		constexpr BlockState RedTerracotta()
		{
			return 6861;
		}
	}
	namespace RedTulip
	{
		constexpr BlockState RedTulip()
		{
			return 1417;
		}
	}
	namespace RedWallBanner
	{
		constexpr BlockState RedWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8209;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8210;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8211;
			else return 8212;
		}
		BlockState RedWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace RedWool
	{
		constexpr BlockState RedWool()
		{
			return 1398;
		}
	}
	namespace RedstoneBlock
	{
		constexpr BlockState RedstoneBlock()
		{
			return 6726;
		}
	}
	namespace RedstoneLamp
	{
		constexpr BlockState RedstoneLamp(const bool Lit)
		{
			if (Lit) return 5156;
			else return 5157;
		}
		BlockState RedstoneLamp();
		bool Lit(BlockState Block);
	}
	namespace RedstoneOre
	{
		constexpr BlockState RedstoneOre(const bool Lit)
		{
			if (Lit) return 3885;
			else return 3886;
		}
		BlockState RedstoneOre();
		bool Lit(BlockState Block);
	}
	namespace RedstoneTorch
	{
		constexpr BlockState RedstoneTorch(const bool Lit)
		{
			if (Lit) return 3887;
			else return 3888;
		}
		BlockState RedstoneTorch();
		bool Lit(BlockState Block);
	}
	namespace RedstoneWallTorch
	{
		constexpr BlockState RedstoneWallTorch(const eBlockFace Facing, const bool Lit)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Lit) return 3889;
				else return 3890;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Lit) return 3891;
				else return 3892;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Lit) return 3893;
				else return 3894;
			else
				if (Lit) return 3895;
				else return 3896;
		}
		BlockState RedstoneWallTorch();
		eBlockFace Facing(BlockState Block);
		bool Lit(BlockState Block);
	}
	namespace RedstoneWire
	{
		enum class East
		{
			Up,
			Side,
			None
		};
		enum class North
		{
			Up,
			Side,
			None
		};
		enum class South
		{
			Up,
			Side,
			None
		};
		enum class West
		{
			Up,
			Side,
			None
		};
		constexpr BlockState RedstoneWire(const enum East East, const enum North North, const unsigned char Power, const enum South South, const enum West West)
		{
			if (East == East::Up)
				if (North == North::Up)
					if (Power == 0)
						if (South == South::Up)
							if (West == West::Up) return 2058;
							else if (West == West::Side) return 2059;
							else return 2060;
						else if (South == South::Side)
							if (West == West::Up) return 2061;
							else if (West == West::Side) return 2062;
							else return 2063;
						else
							if (West == West::Up) return 2064;
							else if (West == West::Side) return 2065;
							else return 2066;
					else if (Power == 1)
						if (South == South::Up)
							if (West == West::Up) return 2067;
							else if (West == West::Side) return 2068;
							else return 2069;
						else if (South == South::Side)
							if (West == West::Up) return 2070;
							else if (West == West::Side) return 2071;
							else return 2072;
						else
							if (West == West::Up) return 2073;
							else if (West == West::Side) return 2074;
							else return 2075;
					else if (Power == 2)
						if (South == South::Up)
							if (West == West::Up) return 2076;
							else if (West == West::Side) return 2077;
							else return 2078;
						else if (South == South::Side)
							if (West == West::Up) return 2079;
							else if (West == West::Side) return 2080;
							else return 2081;
						else
							if (West == West::Up) return 2082;
							else if (West == West::Side) return 2083;
							else return 2084;
					else if (Power == 3)
						if (South == South::Up)
							if (West == West::Up) return 2085;
							else if (West == West::Side) return 2086;
							else return 2087;
						else if (South == South::Side)
							if (West == West::Up) return 2088;
							else if (West == West::Side) return 2089;
							else return 2090;
						else
							if (West == West::Up) return 2091;
							else if (West == West::Side) return 2092;
							else return 2093;
					else if (Power == 4)
						if (South == South::Up)
							if (West == West::Up) return 2094;
							else if (West == West::Side) return 2095;
							else return 2096;
						else if (South == South::Side)
							if (West == West::Up) return 2097;
							else if (West == West::Side) return 2098;
							else return 2099;
						else
							if (West == West::Up) return 2100;
							else if (West == West::Side) return 2101;
							else return 2102;
					else if (Power == 5)
						if (South == South::Up)
							if (West == West::Up) return 2103;
							else if (West == West::Side) return 2104;
							else return 2105;
						else if (South == South::Side)
							if (West == West::Up) return 2106;
							else if (West == West::Side) return 2107;
							else return 2108;
						else
							if (West == West::Up) return 2109;
							else if (West == West::Side) return 2110;
							else return 2111;
					else if (Power == 6)
						if (South == South::Up)
							if (West == West::Up) return 2112;
							else if (West == West::Side) return 2113;
							else return 2114;
						else if (South == South::Side)
							if (West == West::Up) return 2115;
							else if (West == West::Side) return 2116;
							else return 2117;
						else
							if (West == West::Up) return 2118;
							else if (West == West::Side) return 2119;
							else return 2120;
					else if (Power == 7)
						if (South == South::Up)
							if (West == West::Up) return 2121;
							else if (West == West::Side) return 2122;
							else return 2123;
						else if (South == South::Side)
							if (West == West::Up) return 2124;
							else if (West == West::Side) return 2125;
							else return 2126;
						else
							if (West == West::Up) return 2127;
							else if (West == West::Side) return 2128;
							else return 2129;
					else if (Power == 8)
						if (South == South::Up)
							if (West == West::Up) return 2130;
							else if (West == West::Side) return 2131;
							else return 2132;
						else if (South == South::Side)
							if (West == West::Up) return 2133;
							else if (West == West::Side) return 2134;
							else return 2135;
						else
							if (West == West::Up) return 2136;
							else if (West == West::Side) return 2137;
							else return 2138;
					else if (Power == 9)
						if (South == South::Up)
							if (West == West::Up) return 2139;
							else if (West == West::Side) return 2140;
							else return 2141;
						else if (South == South::Side)
							if (West == West::Up) return 2142;
							else if (West == West::Side) return 2143;
							else return 2144;
						else
							if (West == West::Up) return 2145;
							else if (West == West::Side) return 2146;
							else return 2147;
					else if (Power == 10)
						if (South == South::Up)
							if (West == West::Up) return 2148;
							else if (West == West::Side) return 2149;
							else return 2150;
						else if (South == South::Side)
							if (West == West::Up) return 2151;
							else if (West == West::Side) return 2152;
							else return 2153;
						else
							if (West == West::Up) return 2154;
							else if (West == West::Side) return 2155;
							else return 2156;
					else if (Power == 11)
						if (South == South::Up)
							if (West == West::Up) return 2157;
							else if (West == West::Side) return 2158;
							else return 2159;
						else if (South == South::Side)
							if (West == West::Up) return 2160;
							else if (West == West::Side) return 2161;
							else return 2162;
						else
							if (West == West::Up) return 2163;
							else if (West == West::Side) return 2164;
							else return 2165;
					else if (Power == 12)
						if (South == South::Up)
							if (West == West::Up) return 2166;
							else if (West == West::Side) return 2167;
							else return 2168;
						else if (South == South::Side)
							if (West == West::Up) return 2169;
							else if (West == West::Side) return 2170;
							else return 2171;
						else
							if (West == West::Up) return 2172;
							else if (West == West::Side) return 2173;
							else return 2174;
					else if (Power == 13)
						if (South == South::Up)
							if (West == West::Up) return 2175;
							else if (West == West::Side) return 2176;
							else return 2177;
						else if (South == South::Side)
							if (West == West::Up) return 2178;
							else if (West == West::Side) return 2179;
							else return 2180;
						else
							if (West == West::Up) return 2181;
							else if (West == West::Side) return 2182;
							else return 2183;
					else if (Power == 14)
						if (South == South::Up)
							if (West == West::Up) return 2184;
							else if (West == West::Side) return 2185;
							else return 2186;
						else if (South == South::Side)
							if (West == West::Up) return 2187;
							else if (West == West::Side) return 2188;
							else return 2189;
						else
							if (West == West::Up) return 2190;
							else if (West == West::Side) return 2191;
							else return 2192;
					else
						if (South == South::Up)
							if (West == West::Up) return 2193;
							else if (West == West::Side) return 2194;
							else return 2195;
						else if (South == South::Side)
							if (West == West::Up) return 2196;
							else if (West == West::Side) return 2197;
							else return 2198;
						else
							if (West == West::Up) return 2199;
							else if (West == West::Side) return 2200;
							else return 2201;
				else if (North == North::Side)
					if (Power == 0)
						if (South == South::Up)
							if (West == West::Up) return 2202;
							else if (West == West::Side) return 2203;
							else return 2204;
						else if (South == South::Side)
							if (West == West::Up) return 2205;
							else if (West == West::Side) return 2206;
							else return 2207;
						else
							if (West == West::Up) return 2208;
							else if (West == West::Side) return 2209;
							else return 2210;
					else if (Power == 1)
						if (South == South::Up)
							if (West == West::Up) return 2211;
							else if (West == West::Side) return 2212;
							else return 2213;
						else if (South == South::Side)
							if (West == West::Up) return 2214;
							else if (West == West::Side) return 2215;
							else return 2216;
						else
							if (West == West::Up) return 2217;
							else if (West == West::Side) return 2218;
							else return 2219;
					else if (Power == 2)
						if (South == South::Up)
							if (West == West::Up) return 2220;
							else if (West == West::Side) return 2221;
							else return 2222;
						else if (South == South::Side)
							if (West == West::Up) return 2223;
							else if (West == West::Side) return 2224;
							else return 2225;
						else
							if (West == West::Up) return 2226;
							else if (West == West::Side) return 2227;
							else return 2228;
					else if (Power == 3)
						if (South == South::Up)
							if (West == West::Up) return 2229;
							else if (West == West::Side) return 2230;
							else return 2231;
						else if (South == South::Side)
							if (West == West::Up) return 2232;
							else if (West == West::Side) return 2233;
							else return 2234;
						else
							if (West == West::Up) return 2235;
							else if (West == West::Side) return 2236;
							else return 2237;
					else if (Power == 4)
						if (South == South::Up)
							if (West == West::Up) return 2238;
							else if (West == West::Side) return 2239;
							else return 2240;
						else if (South == South::Side)
							if (West == West::Up) return 2241;
							else if (West == West::Side) return 2242;
							else return 2243;
						else
							if (West == West::Up) return 2244;
							else if (West == West::Side) return 2245;
							else return 2246;
					else if (Power == 5)
						if (South == South::Up)
							if (West == West::Up) return 2247;
							else if (West == West::Side) return 2248;
							else return 2249;
						else if (South == South::Side)
							if (West == West::Up) return 2250;
							else if (West == West::Side) return 2251;
							else return 2252;
						else
							if (West == West::Up) return 2253;
							else if (West == West::Side) return 2254;
							else return 2255;
					else if (Power == 6)
						if (South == South::Up)
							if (West == West::Up) return 2256;
							else if (West == West::Side) return 2257;
							else return 2258;
						else if (South == South::Side)
							if (West == West::Up) return 2259;
							else if (West == West::Side) return 2260;
							else return 2261;
						else
							if (West == West::Up) return 2262;
							else if (West == West::Side) return 2263;
							else return 2264;
					else if (Power == 7)
						if (South == South::Up)
							if (West == West::Up) return 2265;
							else if (West == West::Side) return 2266;
							else return 2267;
						else if (South == South::Side)
							if (West == West::Up) return 2268;
							else if (West == West::Side) return 2269;
							else return 2270;
						else
							if (West == West::Up) return 2271;
							else if (West == West::Side) return 2272;
							else return 2273;
					else if (Power == 8)
						if (South == South::Up)
							if (West == West::Up) return 2274;
							else if (West == West::Side) return 2275;
							else return 2276;
						else if (South == South::Side)
							if (West == West::Up) return 2277;
							else if (West == West::Side) return 2278;
							else return 2279;
						else
							if (West == West::Up) return 2280;
							else if (West == West::Side) return 2281;
							else return 2282;
					else if (Power == 9)
						if (South == South::Up)
							if (West == West::Up) return 2283;
							else if (West == West::Side) return 2284;
							else return 2285;
						else if (South == South::Side)
							if (West == West::Up) return 2286;
							else if (West == West::Side) return 2287;
							else return 2288;
						else
							if (West == West::Up) return 2289;
							else if (West == West::Side) return 2290;
							else return 2291;
					else if (Power == 10)
						if (South == South::Up)
							if (West == West::Up) return 2292;
							else if (West == West::Side) return 2293;
							else return 2294;
						else if (South == South::Side)
							if (West == West::Up) return 2295;
							else if (West == West::Side) return 2296;
							else return 2297;
						else
							if (West == West::Up) return 2298;
							else if (West == West::Side) return 2299;
							else return 2300;
					else if (Power == 11)
						if (South == South::Up)
							if (West == West::Up) return 2301;
							else if (West == West::Side) return 2302;
							else return 2303;
						else if (South == South::Side)
							if (West == West::Up) return 2304;
							else if (West == West::Side) return 2305;
							else return 2306;
						else
							if (West == West::Up) return 2307;
							else if (West == West::Side) return 2308;
							else return 2309;
					else if (Power == 12)
						if (South == South::Up)
							if (West == West::Up) return 2310;
							else if (West == West::Side) return 2311;
							else return 2312;
						else if (South == South::Side)
							if (West == West::Up) return 2313;
							else if (West == West::Side) return 2314;
							else return 2315;
						else
							if (West == West::Up) return 2316;
							else if (West == West::Side) return 2317;
							else return 2318;
					else if (Power == 13)
						if (South == South::Up)
							if (West == West::Up) return 2319;
							else if (West == West::Side) return 2320;
							else return 2321;
						else if (South == South::Side)
							if (West == West::Up) return 2322;
							else if (West == West::Side) return 2323;
							else return 2324;
						else
							if (West == West::Up) return 2325;
							else if (West == West::Side) return 2326;
							else return 2327;
					else if (Power == 14)
						if (South == South::Up)
							if (West == West::Up) return 2328;
							else if (West == West::Side) return 2329;
							else return 2330;
						else if (South == South::Side)
							if (West == West::Up) return 2331;
							else if (West == West::Side) return 2332;
							else return 2333;
						else
							if (West == West::Up) return 2334;
							else if (West == West::Side) return 2335;
							else return 2336;
					else
						if (South == South::Up)
							if (West == West::Up) return 2337;
							else if (West == West::Side) return 2338;
							else return 2339;
						else if (South == South::Side)
							if (West == West::Up) return 2340;
							else if (West == West::Side) return 2341;
							else return 2342;
						else
							if (West == West::Up) return 2343;
							else if (West == West::Side) return 2344;
							else return 2345;
				else
					if (Power == 0)
						if (South == South::Up)
							if (West == West::Up) return 2346;
							else if (West == West::Side) return 2347;
							else return 2348;
						else if (South == South::Side)
							if (West == West::Up) return 2349;
							else if (West == West::Side) return 2350;
							else return 2351;
						else
							if (West == West::Up) return 2352;
							else if (West == West::Side) return 2353;
							else return 2354;
					else if (Power == 1)
						if (South == South::Up)
							if (West == West::Up) return 2355;
							else if (West == West::Side) return 2356;
							else return 2357;
						else if (South == South::Side)
							if (West == West::Up) return 2358;
							else if (West == West::Side) return 2359;
							else return 2360;
						else
							if (West == West::Up) return 2361;
							else if (West == West::Side) return 2362;
							else return 2363;
					else if (Power == 2)
						if (South == South::Up)
							if (West == West::Up) return 2364;
							else if (West == West::Side) return 2365;
							else return 2366;
						else if (South == South::Side)
							if (West == West::Up) return 2367;
							else if (West == West::Side) return 2368;
							else return 2369;
						else
							if (West == West::Up) return 2370;
							else if (West == West::Side) return 2371;
							else return 2372;
					else if (Power == 3)
						if (South == South::Up)
							if (West == West::Up) return 2373;
							else if (West == West::Side) return 2374;
							else return 2375;
						else if (South == South::Side)
							if (West == West::Up) return 2376;
							else if (West == West::Side) return 2377;
							else return 2378;
						else
							if (West == West::Up) return 2379;
							else if (West == West::Side) return 2380;
							else return 2381;
					else if (Power == 4)
						if (South == South::Up)
							if (West == West::Up) return 2382;
							else if (West == West::Side) return 2383;
							else return 2384;
						else if (South == South::Side)
							if (West == West::Up) return 2385;
							else if (West == West::Side) return 2386;
							else return 2387;
						else
							if (West == West::Up) return 2388;
							else if (West == West::Side) return 2389;
							else return 2390;
					else if (Power == 5)
						if (South == South::Up)
							if (West == West::Up) return 2391;
							else if (West == West::Side) return 2392;
							else return 2393;
						else if (South == South::Side)
							if (West == West::Up) return 2394;
							else if (West == West::Side) return 2395;
							else return 2396;
						else
							if (West == West::Up) return 2397;
							else if (West == West::Side) return 2398;
							else return 2399;
					else if (Power == 6)
						if (South == South::Up)
							if (West == West::Up) return 2400;
							else if (West == West::Side) return 2401;
							else return 2402;
						else if (South == South::Side)
							if (West == West::Up) return 2403;
							else if (West == West::Side) return 2404;
							else return 2405;
						else
							if (West == West::Up) return 2406;
							else if (West == West::Side) return 2407;
							else return 2408;
					else if (Power == 7)
						if (South == South::Up)
							if (West == West::Up) return 2409;
							else if (West == West::Side) return 2410;
							else return 2411;
						else if (South == South::Side)
							if (West == West::Up) return 2412;
							else if (West == West::Side) return 2413;
							else return 2414;
						else
							if (West == West::Up) return 2415;
							else if (West == West::Side) return 2416;
							else return 2417;
					else if (Power == 8)
						if (South == South::Up)
							if (West == West::Up) return 2418;
							else if (West == West::Side) return 2419;
							else return 2420;
						else if (South == South::Side)
							if (West == West::Up) return 2421;
							else if (West == West::Side) return 2422;
							else return 2423;
						else
							if (West == West::Up) return 2424;
							else if (West == West::Side) return 2425;
							else return 2426;
					else if (Power == 9)
						if (South == South::Up)
							if (West == West::Up) return 2427;
							else if (West == West::Side) return 2428;
							else return 2429;
						else if (South == South::Side)
							if (West == West::Up) return 2430;
							else if (West == West::Side) return 2431;
							else return 2432;
						else
							if (West == West::Up) return 2433;
							else if (West == West::Side) return 2434;
							else return 2435;
					else if (Power == 10)
						if (South == South::Up)
							if (West == West::Up) return 2436;
							else if (West == West::Side) return 2437;
							else return 2438;
						else if (South == South::Side)
							if (West == West::Up) return 2439;
							else if (West == West::Side) return 2440;
							else return 2441;
						else
							if (West == West::Up) return 2442;
							else if (West == West::Side) return 2443;
							else return 2444;
					else if (Power == 11)
						if (South == South::Up)
							if (West == West::Up) return 2445;
							else if (West == West::Side) return 2446;
							else return 2447;
						else if (South == South::Side)
							if (West == West::Up) return 2448;
							else if (West == West::Side) return 2449;
							else return 2450;
						else
							if (West == West::Up) return 2451;
							else if (West == West::Side) return 2452;
							else return 2453;
					else if (Power == 12)
						if (South == South::Up)
							if (West == West::Up) return 2454;
							else if (West == West::Side) return 2455;
							else return 2456;
						else if (South == South::Side)
							if (West == West::Up) return 2457;
							else if (West == West::Side) return 2458;
							else return 2459;
						else
							if (West == West::Up) return 2460;
							else if (West == West::Side) return 2461;
							else return 2462;
					else if (Power == 13)
						if (South == South::Up)
							if (West == West::Up) return 2463;
							else if (West == West::Side) return 2464;
							else return 2465;
						else if (South == South::Side)
							if (West == West::Up) return 2466;
							else if (West == West::Side) return 2467;
							else return 2468;
						else
							if (West == West::Up) return 2469;
							else if (West == West::Side) return 2470;
							else return 2471;
					else if (Power == 14)
						if (South == South::Up)
							if (West == West::Up) return 2472;
							else if (West == West::Side) return 2473;
							else return 2474;
						else if (South == South::Side)
							if (West == West::Up) return 2475;
							else if (West == West::Side) return 2476;
							else return 2477;
						else
							if (West == West::Up) return 2478;
							else if (West == West::Side) return 2479;
							else return 2480;
					else
						if (South == South::Up)
							if (West == West::Up) return 2481;
							else if (West == West::Side) return 2482;
							else return 2483;
						else if (South == South::Side)
							if (West == West::Up) return 2484;
							else if (West == West::Side) return 2485;
							else return 2486;
						else
							if (West == West::Up) return 2487;
							else if (West == West::Side) return 2488;
							else return 2489;
			else if (East == East::Side)
				if (North == North::Up)
					if (Power == 0)
						if (South == South::Up)
							if (West == West::Up) return 2490;
							else if (West == West::Side) return 2491;
							else return 2492;
						else if (South == South::Side)
							if (West == West::Up) return 2493;
							else if (West == West::Side) return 2494;
							else return 2495;
						else
							if (West == West::Up) return 2496;
							else if (West == West::Side) return 2497;
							else return 2498;
					else if (Power == 1)
						if (South == South::Up)
							if (West == West::Up) return 2499;
							else if (West == West::Side) return 2500;
							else return 2501;
						else if (South == South::Side)
							if (West == West::Up) return 2502;
							else if (West == West::Side) return 2503;
							else return 2504;
						else
							if (West == West::Up) return 2505;
							else if (West == West::Side) return 2506;
							else return 2507;
					else if (Power == 2)
						if (South == South::Up)
							if (West == West::Up) return 2508;
							else if (West == West::Side) return 2509;
							else return 2510;
						else if (South == South::Side)
							if (West == West::Up) return 2511;
							else if (West == West::Side) return 2512;
							else return 2513;
						else
							if (West == West::Up) return 2514;
							else if (West == West::Side) return 2515;
							else return 2516;
					else if (Power == 3)
						if (South == South::Up)
							if (West == West::Up) return 2517;
							else if (West == West::Side) return 2518;
							else return 2519;
						else if (South == South::Side)
							if (West == West::Up) return 2520;
							else if (West == West::Side) return 2521;
							else return 2522;
						else
							if (West == West::Up) return 2523;
							else if (West == West::Side) return 2524;
							else return 2525;
					else if (Power == 4)
						if (South == South::Up)
							if (West == West::Up) return 2526;
							else if (West == West::Side) return 2527;
							else return 2528;
						else if (South == South::Side)
							if (West == West::Up) return 2529;
							else if (West == West::Side) return 2530;
							else return 2531;
						else
							if (West == West::Up) return 2532;
							else if (West == West::Side) return 2533;
							else return 2534;
					else if (Power == 5)
						if (South == South::Up)
							if (West == West::Up) return 2535;
							else if (West == West::Side) return 2536;
							else return 2537;
						else if (South == South::Side)
							if (West == West::Up) return 2538;
							else if (West == West::Side) return 2539;
							else return 2540;
						else
							if (West == West::Up) return 2541;
							else if (West == West::Side) return 2542;
							else return 2543;
					else if (Power == 6)
						if (South == South::Up)
							if (West == West::Up) return 2544;
							else if (West == West::Side) return 2545;
							else return 2546;
						else if (South == South::Side)
							if (West == West::Up) return 2547;
							else if (West == West::Side) return 2548;
							else return 2549;
						else
							if (West == West::Up) return 2550;
							else if (West == West::Side) return 2551;
							else return 2552;
					else if (Power == 7)
						if (South == South::Up)
							if (West == West::Up) return 2553;
							else if (West == West::Side) return 2554;
							else return 2555;
						else if (South == South::Side)
							if (West == West::Up) return 2556;
							else if (West == West::Side) return 2557;
							else return 2558;
						else
							if (West == West::Up) return 2559;
							else if (West == West::Side) return 2560;
							else return 2561;
					else if (Power == 8)
						if (South == South::Up)
							if (West == West::Up) return 2562;
							else if (West == West::Side) return 2563;
							else return 2564;
						else if (South == South::Side)
							if (West == West::Up) return 2565;
							else if (West == West::Side) return 2566;
							else return 2567;
						else
							if (West == West::Up) return 2568;
							else if (West == West::Side) return 2569;
							else return 2570;
					else if (Power == 9)
						if (South == South::Up)
							if (West == West::Up) return 2571;
							else if (West == West::Side) return 2572;
							else return 2573;
						else if (South == South::Side)
							if (West == West::Up) return 2574;
							else if (West == West::Side) return 2575;
							else return 2576;
						else
							if (West == West::Up) return 2577;
							else if (West == West::Side) return 2578;
							else return 2579;
					else if (Power == 10)
						if (South == South::Up)
							if (West == West::Up) return 2580;
							else if (West == West::Side) return 2581;
							else return 2582;
						else if (South == South::Side)
							if (West == West::Up) return 2583;
							else if (West == West::Side) return 2584;
							else return 2585;
						else
							if (West == West::Up) return 2586;
							else if (West == West::Side) return 2587;
							else return 2588;
					else if (Power == 11)
						if (South == South::Up)
							if (West == West::Up) return 2589;
							else if (West == West::Side) return 2590;
							else return 2591;
						else if (South == South::Side)
							if (West == West::Up) return 2592;
							else if (West == West::Side) return 2593;
							else return 2594;
						else
							if (West == West::Up) return 2595;
							else if (West == West::Side) return 2596;
							else return 2597;
					else if (Power == 12)
						if (South == South::Up)
							if (West == West::Up) return 2598;
							else if (West == West::Side) return 2599;
							else return 2600;
						else if (South == South::Side)
							if (West == West::Up) return 2601;
							else if (West == West::Side) return 2602;
							else return 2603;
						else
							if (West == West::Up) return 2604;
							else if (West == West::Side) return 2605;
							else return 2606;
					else if (Power == 13)
						if (South == South::Up)
							if (West == West::Up) return 2607;
							else if (West == West::Side) return 2608;
							else return 2609;
						else if (South == South::Side)
							if (West == West::Up) return 2610;
							else if (West == West::Side) return 2611;
							else return 2612;
						else
							if (West == West::Up) return 2613;
							else if (West == West::Side) return 2614;
							else return 2615;
					else if (Power == 14)
						if (South == South::Up)
							if (West == West::Up) return 2616;
							else if (West == West::Side) return 2617;
							else return 2618;
						else if (South == South::Side)
							if (West == West::Up) return 2619;
							else if (West == West::Side) return 2620;
							else return 2621;
						else
							if (West == West::Up) return 2622;
							else if (West == West::Side) return 2623;
							else return 2624;
					else
						if (South == South::Up)
							if (West == West::Up) return 2625;
							else if (West == West::Side) return 2626;
							else return 2627;
						else if (South == South::Side)
							if (West == West::Up) return 2628;
							else if (West == West::Side) return 2629;
							else return 2630;
						else
							if (West == West::Up) return 2631;
							else if (West == West::Side) return 2632;
							else return 2633;
				else if (North == North::Side)
					if (Power == 0)
						if (South == South::Up)
							if (West == West::Up) return 2634;
							else if (West == West::Side) return 2635;
							else return 2636;
						else if (South == South::Side)
							if (West == West::Up) return 2637;
							else if (West == West::Side) return 2638;
							else return 2639;
						else
							if (West == West::Up) return 2640;
							else if (West == West::Side) return 2641;
							else return 2642;
					else if (Power == 1)
						if (South == South::Up)
							if (West == West::Up) return 2643;
							else if (West == West::Side) return 2644;
							else return 2645;
						else if (South == South::Side)
							if (West == West::Up) return 2646;
							else if (West == West::Side) return 2647;
							else return 2648;
						else
							if (West == West::Up) return 2649;
							else if (West == West::Side) return 2650;
							else return 2651;
					else if (Power == 2)
						if (South == South::Up)
							if (West == West::Up) return 2652;
							else if (West == West::Side) return 2653;
							else return 2654;
						else if (South == South::Side)
							if (West == West::Up) return 2655;
							else if (West == West::Side) return 2656;
							else return 2657;
						else
							if (West == West::Up) return 2658;
							else if (West == West::Side) return 2659;
							else return 2660;
					else if (Power == 3)
						if (South == South::Up)
							if (West == West::Up) return 2661;
							else if (West == West::Side) return 2662;
							else return 2663;
						else if (South == South::Side)
							if (West == West::Up) return 2664;
							else if (West == West::Side) return 2665;
							else return 2666;
						else
							if (West == West::Up) return 2667;
							else if (West == West::Side) return 2668;
							else return 2669;
					else if (Power == 4)
						if (South == South::Up)
							if (West == West::Up) return 2670;
							else if (West == West::Side) return 2671;
							else return 2672;
						else if (South == South::Side)
							if (West == West::Up) return 2673;
							else if (West == West::Side) return 2674;
							else return 2675;
						else
							if (West == West::Up) return 2676;
							else if (West == West::Side) return 2677;
							else return 2678;
					else if (Power == 5)
						if (South == South::Up)
							if (West == West::Up) return 2679;
							else if (West == West::Side) return 2680;
							else return 2681;
						else if (South == South::Side)
							if (West == West::Up) return 2682;
							else if (West == West::Side) return 2683;
							else return 2684;
						else
							if (West == West::Up) return 2685;
							else if (West == West::Side) return 2686;
							else return 2687;
					else if (Power == 6)
						if (South == South::Up)
							if (West == West::Up) return 2688;
							else if (West == West::Side) return 2689;
							else return 2690;
						else if (South == South::Side)
							if (West == West::Up) return 2691;
							else if (West == West::Side) return 2692;
							else return 2693;
						else
							if (West == West::Up) return 2694;
							else if (West == West::Side) return 2695;
							else return 2696;
					else if (Power == 7)
						if (South == South::Up)
							if (West == West::Up) return 2697;
							else if (West == West::Side) return 2698;
							else return 2699;
						else if (South == South::Side)
							if (West == West::Up) return 2700;
							else if (West == West::Side) return 2701;
							else return 2702;
						else
							if (West == West::Up) return 2703;
							else if (West == West::Side) return 2704;
							else return 2705;
					else if (Power == 8)
						if (South == South::Up)
							if (West == West::Up) return 2706;
							else if (West == West::Side) return 2707;
							else return 2708;
						else if (South == South::Side)
							if (West == West::Up) return 2709;
							else if (West == West::Side) return 2710;
							else return 2711;
						else
							if (West == West::Up) return 2712;
							else if (West == West::Side) return 2713;
							else return 2714;
					else if (Power == 9)
						if (South == South::Up)
							if (West == West::Up) return 2715;
							else if (West == West::Side) return 2716;
							else return 2717;
						else if (South == South::Side)
							if (West == West::Up) return 2718;
							else if (West == West::Side) return 2719;
							else return 2720;
						else
							if (West == West::Up) return 2721;
							else if (West == West::Side) return 2722;
							else return 2723;
					else if (Power == 10)
						if (South == South::Up)
							if (West == West::Up) return 2724;
							else if (West == West::Side) return 2725;
							else return 2726;
						else if (South == South::Side)
							if (West == West::Up) return 2727;
							else if (West == West::Side) return 2728;
							else return 2729;
						else
							if (West == West::Up) return 2730;
							else if (West == West::Side) return 2731;
							else return 2732;
					else if (Power == 11)
						if (South == South::Up)
							if (West == West::Up) return 2733;
							else if (West == West::Side) return 2734;
							else return 2735;
						else if (South == South::Side)
							if (West == West::Up) return 2736;
							else if (West == West::Side) return 2737;
							else return 2738;
						else
							if (West == West::Up) return 2739;
							else if (West == West::Side) return 2740;
							else return 2741;
					else if (Power == 12)
						if (South == South::Up)
							if (West == West::Up) return 2742;
							else if (West == West::Side) return 2743;
							else return 2744;
						else if (South == South::Side)
							if (West == West::Up) return 2745;
							else if (West == West::Side) return 2746;
							else return 2747;
						else
							if (West == West::Up) return 2748;
							else if (West == West::Side) return 2749;
							else return 2750;
					else if (Power == 13)
						if (South == South::Up)
							if (West == West::Up) return 2751;
							else if (West == West::Side) return 2752;
							else return 2753;
						else if (South == South::Side)
							if (West == West::Up) return 2754;
							else if (West == West::Side) return 2755;
							else return 2756;
						else
							if (West == West::Up) return 2757;
							else if (West == West::Side) return 2758;
							else return 2759;
					else if (Power == 14)
						if (South == South::Up)
							if (West == West::Up) return 2760;
							else if (West == West::Side) return 2761;
							else return 2762;
						else if (South == South::Side)
							if (West == West::Up) return 2763;
							else if (West == West::Side) return 2764;
							else return 2765;
						else
							if (West == West::Up) return 2766;
							else if (West == West::Side) return 2767;
							else return 2768;
					else
						if (South == South::Up)
							if (West == West::Up) return 2769;
							else if (West == West::Side) return 2770;
							else return 2771;
						else if (South == South::Side)
							if (West == West::Up) return 2772;
							else if (West == West::Side) return 2773;
							else return 2774;
						else
							if (West == West::Up) return 2775;
							else if (West == West::Side) return 2776;
							else return 2777;
				else
					if (Power == 0)
						if (South == South::Up)
							if (West == West::Up) return 2778;
							else if (West == West::Side) return 2779;
							else return 2780;
						else if (South == South::Side)
							if (West == West::Up) return 2781;
							else if (West == West::Side) return 2782;
							else return 2783;
						else
							if (West == West::Up) return 2784;
							else if (West == West::Side) return 2785;
							else return 2786;
					else if (Power == 1)
						if (South == South::Up)
							if (West == West::Up) return 2787;
							else if (West == West::Side) return 2788;
							else return 2789;
						else if (South == South::Side)
							if (West == West::Up) return 2790;
							else if (West == West::Side) return 2791;
							else return 2792;
						else
							if (West == West::Up) return 2793;
							else if (West == West::Side) return 2794;
							else return 2795;
					else if (Power == 2)
						if (South == South::Up)
							if (West == West::Up) return 2796;
							else if (West == West::Side) return 2797;
							else return 2798;
						else if (South == South::Side)
							if (West == West::Up) return 2799;
							else if (West == West::Side) return 2800;
							else return 2801;
						else
							if (West == West::Up) return 2802;
							else if (West == West::Side) return 2803;
							else return 2804;
					else if (Power == 3)
						if (South == South::Up)
							if (West == West::Up) return 2805;
							else if (West == West::Side) return 2806;
							else return 2807;
						else if (South == South::Side)
							if (West == West::Up) return 2808;
							else if (West == West::Side) return 2809;
							else return 2810;
						else
							if (West == West::Up) return 2811;
							else if (West == West::Side) return 2812;
							else return 2813;
					else if (Power == 4)
						if (South == South::Up)
							if (West == West::Up) return 2814;
							else if (West == West::Side) return 2815;
							else return 2816;
						else if (South == South::Side)
							if (West == West::Up) return 2817;
							else if (West == West::Side) return 2818;
							else return 2819;
						else
							if (West == West::Up) return 2820;
							else if (West == West::Side) return 2821;
							else return 2822;
					else if (Power == 5)
						if (South == South::Up)
							if (West == West::Up) return 2823;
							else if (West == West::Side) return 2824;
							else return 2825;
						else if (South == South::Side)
							if (West == West::Up) return 2826;
							else if (West == West::Side) return 2827;
							else return 2828;
						else
							if (West == West::Up) return 2829;
							else if (West == West::Side) return 2830;
							else return 2831;
					else if (Power == 6)
						if (South == South::Up)
							if (West == West::Up) return 2832;
							else if (West == West::Side) return 2833;
							else return 2834;
						else if (South == South::Side)
							if (West == West::Up) return 2835;
							else if (West == West::Side) return 2836;
							else return 2837;
						else
							if (West == West::Up) return 2838;
							else if (West == West::Side) return 2839;
							else return 2840;
					else if (Power == 7)
						if (South == South::Up)
							if (West == West::Up) return 2841;
							else if (West == West::Side) return 2842;
							else return 2843;
						else if (South == South::Side)
							if (West == West::Up) return 2844;
							else if (West == West::Side) return 2845;
							else return 2846;
						else
							if (West == West::Up) return 2847;
							else if (West == West::Side) return 2848;
							else return 2849;
					else if (Power == 8)
						if (South == South::Up)
							if (West == West::Up) return 2850;
							else if (West == West::Side) return 2851;
							else return 2852;
						else if (South == South::Side)
							if (West == West::Up) return 2853;
							else if (West == West::Side) return 2854;
							else return 2855;
						else
							if (West == West::Up) return 2856;
							else if (West == West::Side) return 2857;
							else return 2858;
					else if (Power == 9)
						if (South == South::Up)
							if (West == West::Up) return 2859;
							else if (West == West::Side) return 2860;
							else return 2861;
						else if (South == South::Side)
							if (West == West::Up) return 2862;
							else if (West == West::Side) return 2863;
							else return 2864;
						else
							if (West == West::Up) return 2865;
							else if (West == West::Side) return 2866;
							else return 2867;
					else if (Power == 10)
						if (South == South::Up)
							if (West == West::Up) return 2868;
							else if (West == West::Side) return 2869;
							else return 2870;
						else if (South == South::Side)
							if (West == West::Up) return 2871;
							else if (West == West::Side) return 2872;
							else return 2873;
						else
							if (West == West::Up) return 2874;
							else if (West == West::Side) return 2875;
							else return 2876;
					else if (Power == 11)
						if (South == South::Up)
							if (West == West::Up) return 2877;
							else if (West == West::Side) return 2878;
							else return 2879;
						else if (South == South::Side)
							if (West == West::Up) return 2880;
							else if (West == West::Side) return 2881;
							else return 2882;
						else
							if (West == West::Up) return 2883;
							else if (West == West::Side) return 2884;
							else return 2885;
					else if (Power == 12)
						if (South == South::Up)
							if (West == West::Up) return 2886;
							else if (West == West::Side) return 2887;
							else return 2888;
						else if (South == South::Side)
							if (West == West::Up) return 2889;
							else if (West == West::Side) return 2890;
							else return 2891;
						else
							if (West == West::Up) return 2892;
							else if (West == West::Side) return 2893;
							else return 2894;
					else if (Power == 13)
						if (South == South::Up)
							if (West == West::Up) return 2895;
							else if (West == West::Side) return 2896;
							else return 2897;
						else if (South == South::Side)
							if (West == West::Up) return 2898;
							else if (West == West::Side) return 2899;
							else return 2900;
						else
							if (West == West::Up) return 2901;
							else if (West == West::Side) return 2902;
							else return 2903;
					else if (Power == 14)
						if (South == South::Up)
							if (West == West::Up) return 2904;
							else if (West == West::Side) return 2905;
							else return 2906;
						else if (South == South::Side)
							if (West == West::Up) return 2907;
							else if (West == West::Side) return 2908;
							else return 2909;
						else
							if (West == West::Up) return 2910;
							else if (West == West::Side) return 2911;
							else return 2912;
					else
						if (South == South::Up)
							if (West == West::Up) return 2913;
							else if (West == West::Side) return 2914;
							else return 2915;
						else if (South == South::Side)
							if (West == West::Up) return 2916;
							else if (West == West::Side) return 2917;
							else return 2918;
						else
							if (West == West::Up) return 2919;
							else if (West == West::Side) return 2920;
							else return 2921;
			else
				if (North == North::Up)
					if (Power == 0)
						if (South == South::Up)
							if (West == West::Up) return 2922;
							else if (West == West::Side) return 2923;
							else return 2924;
						else if (South == South::Side)
							if (West == West::Up) return 2925;
							else if (West == West::Side) return 2926;
							else return 2927;
						else
							if (West == West::Up) return 2928;
							else if (West == West::Side) return 2929;
							else return 2930;
					else if (Power == 1)
						if (South == South::Up)
							if (West == West::Up) return 2931;
							else if (West == West::Side) return 2932;
							else return 2933;
						else if (South == South::Side)
							if (West == West::Up) return 2934;
							else if (West == West::Side) return 2935;
							else return 2936;
						else
							if (West == West::Up) return 2937;
							else if (West == West::Side) return 2938;
							else return 2939;
					else if (Power == 2)
						if (South == South::Up)
							if (West == West::Up) return 2940;
							else if (West == West::Side) return 2941;
							else return 2942;
						else if (South == South::Side)
							if (West == West::Up) return 2943;
							else if (West == West::Side) return 2944;
							else return 2945;
						else
							if (West == West::Up) return 2946;
							else if (West == West::Side) return 2947;
							else return 2948;
					else if (Power == 3)
						if (South == South::Up)
							if (West == West::Up) return 2949;
							else if (West == West::Side) return 2950;
							else return 2951;
						else if (South == South::Side)
							if (West == West::Up) return 2952;
							else if (West == West::Side) return 2953;
							else return 2954;
						else
							if (West == West::Up) return 2955;
							else if (West == West::Side) return 2956;
							else return 2957;
					else if (Power == 4)
						if (South == South::Up)
							if (West == West::Up) return 2958;
							else if (West == West::Side) return 2959;
							else return 2960;
						else if (South == South::Side)
							if (West == West::Up) return 2961;
							else if (West == West::Side) return 2962;
							else return 2963;
						else
							if (West == West::Up) return 2964;
							else if (West == West::Side) return 2965;
							else return 2966;
					else if (Power == 5)
						if (South == South::Up)
							if (West == West::Up) return 2967;
							else if (West == West::Side) return 2968;
							else return 2969;
						else if (South == South::Side)
							if (West == West::Up) return 2970;
							else if (West == West::Side) return 2971;
							else return 2972;
						else
							if (West == West::Up) return 2973;
							else if (West == West::Side) return 2974;
							else return 2975;
					else if (Power == 6)
						if (South == South::Up)
							if (West == West::Up) return 2976;
							else if (West == West::Side) return 2977;
							else return 2978;
						else if (South == South::Side)
							if (West == West::Up) return 2979;
							else if (West == West::Side) return 2980;
							else return 2981;
						else
							if (West == West::Up) return 2982;
							else if (West == West::Side) return 2983;
							else return 2984;
					else if (Power == 7)
						if (South == South::Up)
							if (West == West::Up) return 2985;
							else if (West == West::Side) return 2986;
							else return 2987;
						else if (South == South::Side)
							if (West == West::Up) return 2988;
							else if (West == West::Side) return 2989;
							else return 2990;
						else
							if (West == West::Up) return 2991;
							else if (West == West::Side) return 2992;
							else return 2993;
					else if (Power == 8)
						if (South == South::Up)
							if (West == West::Up) return 2994;
							else if (West == West::Side) return 2995;
							else return 2996;
						else if (South == South::Side)
							if (West == West::Up) return 2997;
							else if (West == West::Side) return 2998;
							else return 2999;
						else
							if (West == West::Up) return 3000;
							else if (West == West::Side) return 3001;
							else return 3002;
					else if (Power == 9)
						if (South == South::Up)
							if (West == West::Up) return 3003;
							else if (West == West::Side) return 3004;
							else return 3005;
						else if (South == South::Side)
							if (West == West::Up) return 3006;
							else if (West == West::Side) return 3007;
							else return 3008;
						else
							if (West == West::Up) return 3009;
							else if (West == West::Side) return 3010;
							else return 3011;
					else if (Power == 10)
						if (South == South::Up)
							if (West == West::Up) return 3012;
							else if (West == West::Side) return 3013;
							else return 3014;
						else if (South == South::Side)
							if (West == West::Up) return 3015;
							else if (West == West::Side) return 3016;
							else return 3017;
						else
							if (West == West::Up) return 3018;
							else if (West == West::Side) return 3019;
							else return 3020;
					else if (Power == 11)
						if (South == South::Up)
							if (West == West::Up) return 3021;
							else if (West == West::Side) return 3022;
							else return 3023;
						else if (South == South::Side)
							if (West == West::Up) return 3024;
							else if (West == West::Side) return 3025;
							else return 3026;
						else
							if (West == West::Up) return 3027;
							else if (West == West::Side) return 3028;
							else return 3029;
					else if (Power == 12)
						if (South == South::Up)
							if (West == West::Up) return 3030;
							else if (West == West::Side) return 3031;
							else return 3032;
						else if (South == South::Side)
							if (West == West::Up) return 3033;
							else if (West == West::Side) return 3034;
							else return 3035;
						else
							if (West == West::Up) return 3036;
							else if (West == West::Side) return 3037;
							else return 3038;
					else if (Power == 13)
						if (South == South::Up)
							if (West == West::Up) return 3039;
							else if (West == West::Side) return 3040;
							else return 3041;
						else if (South == South::Side)
							if (West == West::Up) return 3042;
							else if (West == West::Side) return 3043;
							else return 3044;
						else
							if (West == West::Up) return 3045;
							else if (West == West::Side) return 3046;
							else return 3047;
					else if (Power == 14)
						if (South == South::Up)
							if (West == West::Up) return 3048;
							else if (West == West::Side) return 3049;
							else return 3050;
						else if (South == South::Side)
							if (West == West::Up) return 3051;
							else if (West == West::Side) return 3052;
							else return 3053;
						else
							if (West == West::Up) return 3054;
							else if (West == West::Side) return 3055;
							else return 3056;
					else
						if (South == South::Up)
							if (West == West::Up) return 3057;
							else if (West == West::Side) return 3058;
							else return 3059;
						else if (South == South::Side)
							if (West == West::Up) return 3060;
							else if (West == West::Side) return 3061;
							else return 3062;
						else
							if (West == West::Up) return 3063;
							else if (West == West::Side) return 3064;
							else return 3065;
				else if (North == North::Side)
					if (Power == 0)
						if (South == South::Up)
							if (West == West::Up) return 3066;
							else if (West == West::Side) return 3067;
							else return 3068;
						else if (South == South::Side)
							if (West == West::Up) return 3069;
							else if (West == West::Side) return 3070;
							else return 3071;
						else
							if (West == West::Up) return 3072;
							else if (West == West::Side) return 3073;
							else return 3074;
					else if (Power == 1)
						if (South == South::Up)
							if (West == West::Up) return 3075;
							else if (West == West::Side) return 3076;
							else return 3077;
						else if (South == South::Side)
							if (West == West::Up) return 3078;
							else if (West == West::Side) return 3079;
							else return 3080;
						else
							if (West == West::Up) return 3081;
							else if (West == West::Side) return 3082;
							else return 3083;
					else if (Power == 2)
						if (South == South::Up)
							if (West == West::Up) return 3084;
							else if (West == West::Side) return 3085;
							else return 3086;
						else if (South == South::Side)
							if (West == West::Up) return 3087;
							else if (West == West::Side) return 3088;
							else return 3089;
						else
							if (West == West::Up) return 3090;
							else if (West == West::Side) return 3091;
							else return 3092;
					else if (Power == 3)
						if (South == South::Up)
							if (West == West::Up) return 3093;
							else if (West == West::Side) return 3094;
							else return 3095;
						else if (South == South::Side)
							if (West == West::Up) return 3096;
							else if (West == West::Side) return 3097;
							else return 3098;
						else
							if (West == West::Up) return 3099;
							else if (West == West::Side) return 3100;
							else return 3101;
					else if (Power == 4)
						if (South == South::Up)
							if (West == West::Up) return 3102;
							else if (West == West::Side) return 3103;
							else return 3104;
						else if (South == South::Side)
							if (West == West::Up) return 3105;
							else if (West == West::Side) return 3106;
							else return 3107;
						else
							if (West == West::Up) return 3108;
							else if (West == West::Side) return 3109;
							else return 3110;
					else if (Power == 5)
						if (South == South::Up)
							if (West == West::Up) return 3111;
							else if (West == West::Side) return 3112;
							else return 3113;
						else if (South == South::Side)
							if (West == West::Up) return 3114;
							else if (West == West::Side) return 3115;
							else return 3116;
						else
							if (West == West::Up) return 3117;
							else if (West == West::Side) return 3118;
							else return 3119;
					else if (Power == 6)
						if (South == South::Up)
							if (West == West::Up) return 3120;
							else if (West == West::Side) return 3121;
							else return 3122;
						else if (South == South::Side)
							if (West == West::Up) return 3123;
							else if (West == West::Side) return 3124;
							else return 3125;
						else
							if (West == West::Up) return 3126;
							else if (West == West::Side) return 3127;
							else return 3128;
					else if (Power == 7)
						if (South == South::Up)
							if (West == West::Up) return 3129;
							else if (West == West::Side) return 3130;
							else return 3131;
						else if (South == South::Side)
							if (West == West::Up) return 3132;
							else if (West == West::Side) return 3133;
							else return 3134;
						else
							if (West == West::Up) return 3135;
							else if (West == West::Side) return 3136;
							else return 3137;
					else if (Power == 8)
						if (South == South::Up)
							if (West == West::Up) return 3138;
							else if (West == West::Side) return 3139;
							else return 3140;
						else if (South == South::Side)
							if (West == West::Up) return 3141;
							else if (West == West::Side) return 3142;
							else return 3143;
						else
							if (West == West::Up) return 3144;
							else if (West == West::Side) return 3145;
							else return 3146;
					else if (Power == 9)
						if (South == South::Up)
							if (West == West::Up) return 3147;
							else if (West == West::Side) return 3148;
							else return 3149;
						else if (South == South::Side)
							if (West == West::Up) return 3150;
							else if (West == West::Side) return 3151;
							else return 3152;
						else
							if (West == West::Up) return 3153;
							else if (West == West::Side) return 3154;
							else return 3155;
					else if (Power == 10)
						if (South == South::Up)
							if (West == West::Up) return 3156;
							else if (West == West::Side) return 3157;
							else return 3158;
						else if (South == South::Side)
							if (West == West::Up) return 3159;
							else if (West == West::Side) return 3160;
							else return 3161;
						else
							if (West == West::Up) return 3162;
							else if (West == West::Side) return 3163;
							else return 3164;
					else if (Power == 11)
						if (South == South::Up)
							if (West == West::Up) return 3165;
							else if (West == West::Side) return 3166;
							else return 3167;
						else if (South == South::Side)
							if (West == West::Up) return 3168;
							else if (West == West::Side) return 3169;
							else return 3170;
						else
							if (West == West::Up) return 3171;
							else if (West == West::Side) return 3172;
							else return 3173;
					else if (Power == 12)
						if (South == South::Up)
							if (West == West::Up) return 3174;
							else if (West == West::Side) return 3175;
							else return 3176;
						else if (South == South::Side)
							if (West == West::Up) return 3177;
							else if (West == West::Side) return 3178;
							else return 3179;
						else
							if (West == West::Up) return 3180;
							else if (West == West::Side) return 3181;
							else return 3182;
					else if (Power == 13)
						if (South == South::Up)
							if (West == West::Up) return 3183;
							else if (West == West::Side) return 3184;
							else return 3185;
						else if (South == South::Side)
							if (West == West::Up) return 3186;
							else if (West == West::Side) return 3187;
							else return 3188;
						else
							if (West == West::Up) return 3189;
							else if (West == West::Side) return 3190;
							else return 3191;
					else if (Power == 14)
						if (South == South::Up)
							if (West == West::Up) return 3192;
							else if (West == West::Side) return 3193;
							else return 3194;
						else if (South == South::Side)
							if (West == West::Up) return 3195;
							else if (West == West::Side) return 3196;
							else return 3197;
						else
							if (West == West::Up) return 3198;
							else if (West == West::Side) return 3199;
							else return 3200;
					else
						if (South == South::Up)
							if (West == West::Up) return 3201;
							else if (West == West::Side) return 3202;
							else return 3203;
						else if (South == South::Side)
							if (West == West::Up) return 3204;
							else if (West == West::Side) return 3205;
							else return 3206;
						else
							if (West == West::Up) return 3207;
							else if (West == West::Side) return 3208;
							else return 3209;
				else
					if (Power == 0)
						if (South == South::Up)
							if (West == West::Up) return 3210;
							else if (West == West::Side) return 3211;
							else return 3212;
						else if (South == South::Side)
							if (West == West::Up) return 3213;
							else if (West == West::Side) return 3214;
							else return 3215;
						else
							if (West == West::Up) return 3216;
							else if (West == West::Side) return 3217;
							else return 3218;
					else if (Power == 1)
						if (South == South::Up)
							if (West == West::Up) return 3219;
							else if (West == West::Side) return 3220;
							else return 3221;
						else if (South == South::Side)
							if (West == West::Up) return 3222;
							else if (West == West::Side) return 3223;
							else return 3224;
						else
							if (West == West::Up) return 3225;
							else if (West == West::Side) return 3226;
							else return 3227;
					else if (Power == 2)
						if (South == South::Up)
							if (West == West::Up) return 3228;
							else if (West == West::Side) return 3229;
							else return 3230;
						else if (South == South::Side)
							if (West == West::Up) return 3231;
							else if (West == West::Side) return 3232;
							else return 3233;
						else
							if (West == West::Up) return 3234;
							else if (West == West::Side) return 3235;
							else return 3236;
					else if (Power == 3)
						if (South == South::Up)
							if (West == West::Up) return 3237;
							else if (West == West::Side) return 3238;
							else return 3239;
						else if (South == South::Side)
							if (West == West::Up) return 3240;
							else if (West == West::Side) return 3241;
							else return 3242;
						else
							if (West == West::Up) return 3243;
							else if (West == West::Side) return 3244;
							else return 3245;
					else if (Power == 4)
						if (South == South::Up)
							if (West == West::Up) return 3246;
							else if (West == West::Side) return 3247;
							else return 3248;
						else if (South == South::Side)
							if (West == West::Up) return 3249;
							else if (West == West::Side) return 3250;
							else return 3251;
						else
							if (West == West::Up) return 3252;
							else if (West == West::Side) return 3253;
							else return 3254;
					else if (Power == 5)
						if (South == South::Up)
							if (West == West::Up) return 3255;
							else if (West == West::Side) return 3256;
							else return 3257;
						else if (South == South::Side)
							if (West == West::Up) return 3258;
							else if (West == West::Side) return 3259;
							else return 3260;
						else
							if (West == West::Up) return 3261;
							else if (West == West::Side) return 3262;
							else return 3263;
					else if (Power == 6)
						if (South == South::Up)
							if (West == West::Up) return 3264;
							else if (West == West::Side) return 3265;
							else return 3266;
						else if (South == South::Side)
							if (West == West::Up) return 3267;
							else if (West == West::Side) return 3268;
							else return 3269;
						else
							if (West == West::Up) return 3270;
							else if (West == West::Side) return 3271;
							else return 3272;
					else if (Power == 7)
						if (South == South::Up)
							if (West == West::Up) return 3273;
							else if (West == West::Side) return 3274;
							else return 3275;
						else if (South == South::Side)
							if (West == West::Up) return 3276;
							else if (West == West::Side) return 3277;
							else return 3278;
						else
							if (West == West::Up) return 3279;
							else if (West == West::Side) return 3280;
							else return 3281;
					else if (Power == 8)
						if (South == South::Up)
							if (West == West::Up) return 3282;
							else if (West == West::Side) return 3283;
							else return 3284;
						else if (South == South::Side)
							if (West == West::Up) return 3285;
							else if (West == West::Side) return 3286;
							else return 3287;
						else
							if (West == West::Up) return 3288;
							else if (West == West::Side) return 3289;
							else return 3290;
					else if (Power == 9)
						if (South == South::Up)
							if (West == West::Up) return 3291;
							else if (West == West::Side) return 3292;
							else return 3293;
						else if (South == South::Side)
							if (West == West::Up) return 3294;
							else if (West == West::Side) return 3295;
							else return 3296;
						else
							if (West == West::Up) return 3297;
							else if (West == West::Side) return 3298;
							else return 3299;
					else if (Power == 10)
						if (South == South::Up)
							if (West == West::Up) return 3300;
							else if (West == West::Side) return 3301;
							else return 3302;
						else if (South == South::Side)
							if (West == West::Up) return 3303;
							else if (West == West::Side) return 3304;
							else return 3305;
						else
							if (West == West::Up) return 3306;
							else if (West == West::Side) return 3307;
							else return 3308;
					else if (Power == 11)
						if (South == South::Up)
							if (West == West::Up) return 3309;
							else if (West == West::Side) return 3310;
							else return 3311;
						else if (South == South::Side)
							if (West == West::Up) return 3312;
							else if (West == West::Side) return 3313;
							else return 3314;
						else
							if (West == West::Up) return 3315;
							else if (West == West::Side) return 3316;
							else return 3317;
					else if (Power == 12)
						if (South == South::Up)
							if (West == West::Up) return 3318;
							else if (West == West::Side) return 3319;
							else return 3320;
						else if (South == South::Side)
							if (West == West::Up) return 3321;
							else if (West == West::Side) return 3322;
							else return 3323;
						else
							if (West == West::Up) return 3324;
							else if (West == West::Side) return 3325;
							else return 3326;
					else if (Power == 13)
						if (South == South::Up)
							if (West == West::Up) return 3327;
							else if (West == West::Side) return 3328;
							else return 3329;
						else if (South == South::Side)
							if (West == West::Up) return 3330;
							else if (West == West::Side) return 3331;
							else return 3332;
						else
							if (West == West::Up) return 3333;
							else if (West == West::Side) return 3334;
							else return 3335;
					else if (Power == 14)
						if (South == South::Up)
							if (West == West::Up) return 3336;
							else if (West == West::Side) return 3337;
							else return 3338;
						else if (South == South::Side)
							if (West == West::Up) return 3339;
							else if (West == West::Side) return 3340;
							else return 3341;
						else
							if (West == West::Up) return 3342;
							else if (West == West::Side) return 3343;
							else return 3344;
					else
						if (South == South::Up)
							if (West == West::Up) return 3345;
							else if (West == West::Side) return 3346;
							else return 3347;
						else if (South == South::Side)
							if (West == West::Up) return 3348;
							else if (West == West::Side) return 3349;
							else return 3350;
						else
							if (West == West::Up) return 3351;
							else if (West == West::Side) return 3352;
							else return 3353;
		}
		BlockState RedstoneWire();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		unsigned char Power(BlockState Block);
		enum South South(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace Repeater
	{
		constexpr BlockState Repeater(const unsigned char Delay, const eBlockFace Facing, const bool Locked, const bool Powered)
		{
			if (Delay == 1)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Locked)
						if (Powered) return 4031;
						else return 4032;
					else
						if (Powered) return 4033;
						else return 4034;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Locked)
						if (Powered) return 4035;
						else return 4036;
					else
						if (Powered) return 4037;
						else return 4038;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Locked)
						if (Powered) return 4039;
						else return 4040;
					else
						if (Powered) return 4041;
						else return 4042;
				else
					if (Locked)
						if (Powered) return 4043;
						else return 4044;
					else
						if (Powered) return 4045;
						else return 4046;
			else if (Delay == 2)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Locked)
						if (Powered) return 4047;
						else return 4048;
					else
						if (Powered) return 4049;
						else return 4050;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Locked)
						if (Powered) return 4051;
						else return 4052;
					else
						if (Powered) return 4053;
						else return 4054;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Locked)
						if (Powered) return 4055;
						else return 4056;
					else
						if (Powered) return 4057;
						else return 4058;
				else
					if (Locked)
						if (Powered) return 4059;
						else return 4060;
					else
						if (Powered) return 4061;
						else return 4062;
			else if (Delay == 3)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Locked)
						if (Powered) return 4063;
						else return 4064;
					else
						if (Powered) return 4065;
						else return 4066;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Locked)
						if (Powered) return 4067;
						else return 4068;
					else
						if (Powered) return 4069;
						else return 4070;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Locked)
						if (Powered) return 4071;
						else return 4072;
					else
						if (Powered) return 4073;
						else return 4074;
				else
					if (Locked)
						if (Powered) return 4075;
						else return 4076;
					else
						if (Powered) return 4077;
						else return 4078;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Locked)
						if (Powered) return 4079;
						else return 4080;
					else
						if (Powered) return 4081;
						else return 4082;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Locked)
						if (Powered) return 4083;
						else return 4084;
					else
						if (Powered) return 4085;
						else return 4086;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Locked)
						if (Powered) return 4087;
						else return 4088;
					else
						if (Powered) return 4089;
						else return 4090;
				else
					if (Locked)
						if (Powered) return 4091;
						else return 4092;
					else
						if (Powered) return 4093;
						else return 4094;
		}
		BlockState Repeater();
		unsigned char Delay(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Locked(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace RepeatingCommandBlock
	{
		constexpr BlockState RepeatingCommandBlock(const bool Conditional, const eBlockFace Facing)
		{
			if (Conditional)
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9225;
				else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9226;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9227;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9228;
				else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9229;
				else return 9230;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9231;
				else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9232;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9233;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9234;
				else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9235;
				else return 9236;
		}
		BlockState RepeatingCommandBlock();
		bool Conditional(BlockState Block);
		eBlockFace Facing(BlockState Block);
	}
	namespace RespawnAnchor
	{
		constexpr BlockState RespawnAnchor(const unsigned char Charges)
		{
			if (Charges == 0) return 15829;
			else if (Charges == 1) return 15830;
			else if (Charges == 2) return 15831;
			else if (Charges == 3) return 15832;
			else return 15833;
		}
		BlockState RespawnAnchor();
		unsigned char Charges(BlockState Block);
	}
	namespace RoseBush
	{
		enum class Half
		{
			Upper,
			Lower
		};
		constexpr BlockState RoseBush(const enum Half Half)
		{
			if (Half == Half::Upper) return 7889;
			else return 7890;
		}
		BlockState RoseBush();
		enum Half Half(BlockState Block);
	}
	namespace Sand
	{
		constexpr BlockState Sand()
		{
			return 66;
		}
	}
	namespace Sandstone
	{
		constexpr BlockState Sandstone()
		{
			return 246;
		}
	}
	namespace SandstoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState SandstoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8349;
			else if (Type == Type::Bottom) return 8351;
			else return 8353;
		}
		BlockState SandstoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace SandstoneStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState SandstoneStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5171;
					else if (Shape == Shape::InnerLeft) return 5173;
					else if (Shape == Shape::InnerRight) return 5175;
					else if (Shape == Shape::OuterLeft) return 5177;
					else return 5179;
				else
					if (Shape == Shape::Straight) return 5181;
					else if (Shape == Shape::InnerLeft) return 5183;
					else if (Shape == Shape::InnerRight) return 5185;
					else if (Shape == Shape::OuterLeft) return 5187;
					else return 5189;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5191;
					else if (Shape == Shape::InnerLeft) return 5193;
					else if (Shape == Shape::InnerRight) return 5195;
					else if (Shape == Shape::OuterLeft) return 5197;
					else return 5199;
				else
					if (Shape == Shape::Straight) return 5201;
					else if (Shape == Shape::InnerLeft) return 5203;
					else if (Shape == Shape::InnerRight) return 5205;
					else if (Shape == Shape::OuterLeft) return 5207;
					else return 5209;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5211;
					else if (Shape == Shape::InnerLeft) return 5213;
					else if (Shape == Shape::InnerRight) return 5215;
					else if (Shape == Shape::OuterLeft) return 5217;
					else return 5219;
				else
					if (Shape == Shape::Straight) return 5221;
					else if (Shape == Shape::InnerLeft) return 5223;
					else if (Shape == Shape::InnerRight) return 5225;
					else if (Shape == Shape::OuterLeft) return 5227;
					else return 5229;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5231;
					else if (Shape == Shape::InnerLeft) return 5233;
					else if (Shape == Shape::InnerRight) return 5235;
					else if (Shape == Shape::OuterLeft) return 5237;
					else return 5239;
				else
					if (Shape == Shape::Straight) return 5241;
					else if (Shape == Shape::InnerLeft) return 5243;
					else if (Shape == Shape::InnerRight) return 5245;
					else if (Shape == Shape::OuterLeft) return 5247;
					else return 5249;
		}
		BlockState SandstoneStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace SandstoneWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState SandstoneWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13786;
							else if (West == West::Low) return 13787;
							else return 13788;
						else
							if (West == West::None) return 13792;
							else if (West == West::Low) return 13793;
							else return 13794;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13798;
							else if (West == West::Low) return 13799;
							else return 13800;
						else
							if (West == West::None) return 13804;
							else if (West == West::Low) return 13805;
							else return 13806;
					else
						if (Up)
							if (West == West::None) return 13810;
							else if (West == West::Low) return 13811;
							else return 13812;
						else
							if (West == West::None) return 13816;
							else if (West == West::Low) return 13817;
							else return 13818;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13822;
							else if (West == West::Low) return 13823;
							else return 13824;
						else
							if (West == West::None) return 13828;
							else if (West == West::Low) return 13829;
							else return 13830;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13834;
							else if (West == West::Low) return 13835;
							else return 13836;
						else
							if (West == West::None) return 13840;
							else if (West == West::Low) return 13841;
							else return 13842;
					else
						if (Up)
							if (West == West::None) return 13846;
							else if (West == West::Low) return 13847;
							else return 13848;
						else
							if (West == West::None) return 13852;
							else if (West == West::Low) return 13853;
							else return 13854;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13858;
							else if (West == West::Low) return 13859;
							else return 13860;
						else
							if (West == West::None) return 13864;
							else if (West == West::Low) return 13865;
							else return 13866;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13870;
							else if (West == West::Low) return 13871;
							else return 13872;
						else
							if (West == West::None) return 13876;
							else if (West == West::Low) return 13877;
							else return 13878;
					else
						if (Up)
							if (West == West::None) return 13882;
							else if (West == West::Low) return 13883;
							else return 13884;
						else
							if (West == West::None) return 13888;
							else if (West == West::Low) return 13889;
							else return 13890;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13894;
							else if (West == West::Low) return 13895;
							else return 13896;
						else
							if (West == West::None) return 13900;
							else if (West == West::Low) return 13901;
							else return 13902;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13906;
							else if (West == West::Low) return 13907;
							else return 13908;
						else
							if (West == West::None) return 13912;
							else if (West == West::Low) return 13913;
							else return 13914;
					else
						if (Up)
							if (West == West::None) return 13918;
							else if (West == West::Low) return 13919;
							else return 13920;
						else
							if (West == West::None) return 13924;
							else if (West == West::Low) return 13925;
							else return 13926;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13930;
							else if (West == West::Low) return 13931;
							else return 13932;
						else
							if (West == West::None) return 13936;
							else if (West == West::Low) return 13937;
							else return 13938;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13942;
							else if (West == West::Low) return 13943;
							else return 13944;
						else
							if (West == West::None) return 13948;
							else if (West == West::Low) return 13949;
							else return 13950;
					else
						if (Up)
							if (West == West::None) return 13954;
							else if (West == West::Low) return 13955;
							else return 13956;
						else
							if (West == West::None) return 13960;
							else if (West == West::Low) return 13961;
							else return 13962;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 13966;
							else if (West == West::Low) return 13967;
							else return 13968;
						else
							if (West == West::None) return 13972;
							else if (West == West::Low) return 13973;
							else return 13974;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 13978;
							else if (West == West::Low) return 13979;
							else return 13980;
						else
							if (West == West::None) return 13984;
							else if (West == West::Low) return 13985;
							else return 13986;
					else
						if (Up)
							if (West == West::None) return 13990;
							else if (West == West::Low) return 13991;
							else return 13992;
						else
							if (West == West::None) return 13996;
							else if (West == West::Low) return 13997;
							else return 13998;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14002;
							else if (West == West::Low) return 14003;
							else return 14004;
						else
							if (West == West::None) return 14008;
							else if (West == West::Low) return 14009;
							else return 14010;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14014;
							else if (West == West::Low) return 14015;
							else return 14016;
						else
							if (West == West::None) return 14020;
							else if (West == West::Low) return 14021;
							else return 14022;
					else
						if (Up)
							if (West == West::None) return 14026;
							else if (West == West::Low) return 14027;
							else return 14028;
						else
							if (West == West::None) return 14032;
							else if (West == West::Low) return 14033;
							else return 14034;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14038;
							else if (West == West::Low) return 14039;
							else return 14040;
						else
							if (West == West::None) return 14044;
							else if (West == West::Low) return 14045;
							else return 14046;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14050;
							else if (West == West::Low) return 14051;
							else return 14052;
						else
							if (West == West::None) return 14056;
							else if (West == West::Low) return 14057;
							else return 14058;
					else
						if (Up)
							if (West == West::None) return 14062;
							else if (West == West::Low) return 14063;
							else return 14064;
						else
							if (West == West::None) return 14068;
							else if (West == West::Low) return 14069;
							else return 14070;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 14074;
							else if (West == West::Low) return 14075;
							else return 14076;
						else
							if (West == West::None) return 14080;
							else if (West == West::Low) return 14081;
							else return 14082;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 14086;
							else if (West == West::Low) return 14087;
							else return 14088;
						else
							if (West == West::None) return 14092;
							else if (West == West::Low) return 14093;
							else return 14094;
					else
						if (Up)
							if (West == West::None) return 14098;
							else if (West == West::Low) return 14099;
							else return 14100;
						else
							if (West == West::None) return 14104;
							else if (West == West::Low) return 14105;
							else return 14106;
		}
		BlockState SandstoneWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace Scaffolding
	{
		constexpr BlockState Scaffolding(const bool Bottom, const unsigned char Distance)
		{
			if (Bottom)
				if (Distance == 0) return 14756;
				else if (Distance == 1) return 14758;
				else if (Distance == 2) return 14760;
				else if (Distance == 3) return 14762;
				else if (Distance == 4) return 14764;
				else if (Distance == 5) return 14766;
				else if (Distance == 6) return 14768;
				else return 14770;
			else
				if (Distance == 0) return 14772;
				else if (Distance == 1) return 14774;
				else if (Distance == 2) return 14776;
				else if (Distance == 3) return 14778;
				else if (Distance == 4) return 14780;
				else if (Distance == 5) return 14782;
				else if (Distance == 6) return 14784;
				else return 14786;
		}
		BlockState Scaffolding();
		bool Bottom(BlockState Block);
		unsigned char Distance(BlockState Block);
	}
	namespace SeaLantern
	{
		constexpr BlockState SeaLantern()
		{
			return 7862;
		}
	}
	namespace SeaPickle
	{
		constexpr BlockState SeaPickle(const unsigned char Pickles)
		{
			if (Pickles == 1) return 9641;
			else if (Pickles == 2) return 9643;
			else if (Pickles == 3) return 9645;
			else return 9647;
		}
		BlockState SeaPickle();
		unsigned char Pickles(BlockState Block);
	}
	namespace Seagrass
	{
		constexpr BlockState Seagrass()
		{
			return 1345;
		}
	}
	namespace Shroomlight
	{
		constexpr BlockState Shroomlight()
		{
			return 14989;
		}
	}
	namespace ShulkerBox
	{
		constexpr BlockState ShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9272;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9273;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9274;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9275;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9276;
			else return 9277;
		}
		BlockState ShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace SkeletonSkull
	{
		constexpr BlockState SkeletonSkull(const unsigned char Rotation)
		{
			if (Rotation == 0) return 6490;
			else if (Rotation == 1) return 6491;
			else if (Rotation == 2) return 6492;
			else if (Rotation == 3) return 6493;
			else if (Rotation == 4) return 6494;
			else if (Rotation == 5) return 6495;
			else if (Rotation == 6) return 6496;
			else if (Rotation == 7) return 6497;
			else if (Rotation == 8) return 6498;
			else if (Rotation == 9) return 6499;
			else if (Rotation == 10) return 6500;
			else if (Rotation == 11) return 6501;
			else if (Rotation == 12) return 6502;
			else if (Rotation == 13) return 6503;
			else if (Rotation == 14) return 6504;
			else return 6505;
		}
		BlockState SkeletonSkull();
		unsigned char Rotation(BlockState Block);
	}
	namespace SkeletonWallSkull
	{
		constexpr BlockState SkeletonWallSkull(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6506;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6507;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6508;
			else return 6509;
		}
		BlockState SkeletonWallSkull();
		eBlockFace Facing(BlockState Block);
	}
	namespace SlimeBlock
	{
		constexpr BlockState SlimeBlock()
		{
			return 7535;
		}
	}
	namespace SmithingTable
	{
		constexpr BlockState SmithingTable()
		{
			return 14849;
		}
	}
	namespace Smoker
	{
		constexpr BlockState Smoker(const eBlockFace Facing, const bool Lit)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Lit) return 14803;
				else return 14804;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Lit) return 14805;
				else return 14806;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Lit) return 14807;
				else return 14808;
			else
				if (Lit) return 14809;
				else return 14810;
		}
		BlockState Smoker();
		eBlockFace Facing(BlockState Block);
		bool Lit(BlockState Block);
	}
	namespace SmoothQuartz
	{
		constexpr BlockState SmoothQuartz()
		{
			return 8416;
		}
	}
	namespace SmoothQuartzSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState SmoothQuartzSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10832;
			else if (Type == Type::Bottom) return 10834;
			else return 10836;
		}
		BlockState SmoothQuartzSlab();
		enum Type Type(BlockState Block);
	}
	namespace SmoothQuartzStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState SmoothQuartzStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10310;
					else if (Shape == Shape::InnerLeft) return 10312;
					else if (Shape == Shape::InnerRight) return 10314;
					else if (Shape == Shape::OuterLeft) return 10316;
					else return 10318;
				else
					if (Shape == Shape::Straight) return 10320;
					else if (Shape == Shape::InnerLeft) return 10322;
					else if (Shape == Shape::InnerRight) return 10324;
					else if (Shape == Shape::OuterLeft) return 10326;
					else return 10328;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10330;
					else if (Shape == Shape::InnerLeft) return 10332;
					else if (Shape == Shape::InnerRight) return 10334;
					else if (Shape == Shape::OuterLeft) return 10336;
					else return 10338;
				else
					if (Shape == Shape::Straight) return 10340;
					else if (Shape == Shape::InnerLeft) return 10342;
					else if (Shape == Shape::InnerRight) return 10344;
					else if (Shape == Shape::OuterLeft) return 10346;
					else return 10348;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10350;
					else if (Shape == Shape::InnerLeft) return 10352;
					else if (Shape == Shape::InnerRight) return 10354;
					else if (Shape == Shape::OuterLeft) return 10356;
					else return 10358;
				else
					if (Shape == Shape::Straight) return 10360;
					else if (Shape == Shape::InnerLeft) return 10362;
					else if (Shape == Shape::InnerRight) return 10364;
					else if (Shape == Shape::OuterLeft) return 10366;
					else return 10368;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10370;
					else if (Shape == Shape::InnerLeft) return 10372;
					else if (Shape == Shape::InnerRight) return 10374;
					else if (Shape == Shape::OuterLeft) return 10376;
					else return 10378;
				else
					if (Shape == Shape::Straight) return 10380;
					else if (Shape == Shape::InnerLeft) return 10382;
					else if (Shape == Shape::InnerRight) return 10384;
					else if (Shape == Shape::OuterLeft) return 10386;
					else return 10388;
		}
		BlockState SmoothQuartzStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace SmoothRedSandstone
	{
		constexpr BlockState SmoothRedSandstone()
		{
			return 8417;
		}
	}
	namespace SmoothRedSandstoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState SmoothRedSandstoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10796;
			else if (Type == Type::Bottom) return 10798;
			else return 10800;
		}
		BlockState SmoothRedSandstoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace SmoothRedSandstoneStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState SmoothRedSandstoneStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9750;
					else if (Shape == Shape::InnerLeft) return 9752;
					else if (Shape == Shape::InnerRight) return 9754;
					else if (Shape == Shape::OuterLeft) return 9756;
					else return 9758;
				else
					if (Shape == Shape::Straight) return 9760;
					else if (Shape == Shape::InnerLeft) return 9762;
					else if (Shape == Shape::InnerRight) return 9764;
					else if (Shape == Shape::OuterLeft) return 9766;
					else return 9768;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9770;
					else if (Shape == Shape::InnerLeft) return 9772;
					else if (Shape == Shape::InnerRight) return 9774;
					else if (Shape == Shape::OuterLeft) return 9776;
					else return 9778;
				else
					if (Shape == Shape::Straight) return 9780;
					else if (Shape == Shape::InnerLeft) return 9782;
					else if (Shape == Shape::InnerRight) return 9784;
					else if (Shape == Shape::OuterLeft) return 9786;
					else return 9788;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9790;
					else if (Shape == Shape::InnerLeft) return 9792;
					else if (Shape == Shape::InnerRight) return 9794;
					else if (Shape == Shape::OuterLeft) return 9796;
					else return 9798;
				else
					if (Shape == Shape::Straight) return 9800;
					else if (Shape == Shape::InnerLeft) return 9802;
					else if (Shape == Shape::InnerRight) return 9804;
					else if (Shape == Shape::OuterLeft) return 9806;
					else return 9808;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 9810;
					else if (Shape == Shape::InnerLeft) return 9812;
					else if (Shape == Shape::InnerRight) return 9814;
					else if (Shape == Shape::OuterLeft) return 9816;
					else return 9818;
				else
					if (Shape == Shape::Straight) return 9820;
					else if (Shape == Shape::InnerLeft) return 9822;
					else if (Shape == Shape::InnerRight) return 9824;
					else if (Shape == Shape::OuterLeft) return 9826;
					else return 9828;
		}
		BlockState SmoothRedSandstoneStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace SmoothSandstone
	{
		constexpr BlockState SmoothSandstone()
		{
			return 8415;
		}
	}
	namespace SmoothSandstoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState SmoothSandstoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 10826;
			else if (Type == Type::Bottom) return 10828;
			else return 10830;
		}
		BlockState SmoothSandstoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace SmoothSandstoneStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState SmoothSandstoneStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10230;
					else if (Shape == Shape::InnerLeft) return 10232;
					else if (Shape == Shape::InnerRight) return 10234;
					else if (Shape == Shape::OuterLeft) return 10236;
					else return 10238;
				else
					if (Shape == Shape::Straight) return 10240;
					else if (Shape == Shape::InnerLeft) return 10242;
					else if (Shape == Shape::InnerRight) return 10244;
					else if (Shape == Shape::OuterLeft) return 10246;
					else return 10248;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10250;
					else if (Shape == Shape::InnerLeft) return 10252;
					else if (Shape == Shape::InnerRight) return 10254;
					else if (Shape == Shape::OuterLeft) return 10256;
					else return 10258;
				else
					if (Shape == Shape::Straight) return 10260;
					else if (Shape == Shape::InnerLeft) return 10262;
					else if (Shape == Shape::InnerRight) return 10264;
					else if (Shape == Shape::OuterLeft) return 10266;
					else return 10268;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10270;
					else if (Shape == Shape::InnerLeft) return 10272;
					else if (Shape == Shape::InnerRight) return 10274;
					else if (Shape == Shape::OuterLeft) return 10276;
					else return 10278;
				else
					if (Shape == Shape::Straight) return 10280;
					else if (Shape == Shape::InnerLeft) return 10282;
					else if (Shape == Shape::InnerRight) return 10284;
					else if (Shape == Shape::OuterLeft) return 10286;
					else return 10288;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10290;
					else if (Shape == Shape::InnerLeft) return 10292;
					else if (Shape == Shape::InnerRight) return 10294;
					else if (Shape == Shape::OuterLeft) return 10296;
					else return 10298;
				else
					if (Shape == Shape::Straight) return 10300;
					else if (Shape == Shape::InnerLeft) return 10302;
					else if (Shape == Shape::InnerRight) return 10304;
					else if (Shape == Shape::OuterLeft) return 10306;
					else return 10308;
		}
		BlockState SmoothSandstoneStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace SmoothStone
	{
		constexpr BlockState SmoothStone()
		{
			return 8414;
		}
	}
	namespace SmoothStoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState SmoothStoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8343;
			else if (Type == Type::Bottom) return 8345;
			else return 8347;
		}
		BlockState SmoothStoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace Snow
	{
		constexpr BlockState Snow(const unsigned char Layers)
		{
			if (Layers == 1) return 3921;
			else if (Layers == 2) return 3922;
			else if (Layers == 3) return 3923;
			else if (Layers == 4) return 3924;
			else if (Layers == 5) return 3925;
			else if (Layers == 6) return 3926;
			else if (Layers == 7) return 3927;
			else return 3928;
		}
		BlockState Snow();
		unsigned char Layers(BlockState Block);
	}
	namespace SnowBlock
	{
		constexpr BlockState SnowBlock()
		{
			return 3930;
		}
	}
	namespace SoulCampfire
	{
		constexpr BlockState SoulCampfire(const eBlockFace Facing, const bool Lit, const bool SignalFire)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Lit)
					if (SignalFire) return 14923;
					else return 14925;
				else
					if (SignalFire) return 14927;
					else return 14929;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Lit)
					if (SignalFire) return 14931;
					else return 14933;
				else
					if (SignalFire) return 14935;
					else return 14937;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Lit)
					if (SignalFire) return 14939;
					else return 14941;
				else
					if (SignalFire) return 14943;
					else return 14945;
			else
				if (Lit)
					if (SignalFire) return 14947;
					else return 14949;
				else
					if (SignalFire) return 14951;
					else return 14953;
		}
		BlockState SoulCampfire();
		eBlockFace Facing(BlockState Block);
		bool Lit(BlockState Block);
		bool SignalFire(BlockState Block);
	}
	namespace SoulFire
	{
		constexpr BlockState SoulFire()
		{
			return 1952;
		}
	}
	namespace SoulLantern
	{
		constexpr BlockState SoulLantern(const bool Hanging)
		{
			if (Hanging) return 14888;
			else return 14889;
		}
		BlockState SoulLantern();
		bool Hanging(BlockState Block);
	}
	namespace SoulSand
	{
		constexpr BlockState SoulSand()
		{
			return 4000;
		}
	}
	namespace SoulSoil
	{
		constexpr BlockState SoulSoil()
		{
			return 4001;
		}
	}
	namespace SoulTorch
	{
		constexpr BlockState SoulTorch()
		{
			return 4008;
		}
	}
	namespace SoulWallTorch
	{
		constexpr BlockState SoulWallTorch(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 4009;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 4010;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 4011;
			else return 4012;
		}
		BlockState SoulWallTorch();
		eBlockFace Facing(BlockState Block);
	}
	namespace Spawner
	{
		constexpr BlockState Spawner()
		{
			return 1953;
		}
	}
	namespace Sponge
	{
		constexpr BlockState Sponge()
		{
			return 229;
		}
	}
	namespace SpruceButton
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState SpruceButton(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6370;
					else return 6371;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6372;
					else return 6373;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6374;
					else return 6375;
				else
					if (Powered) return 6376;
					else return 6377;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6378;
					else return 6379;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6380;
					else return 6381;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6382;
					else return 6383;
				else
					if (Powered) return 6384;
					else return 6385;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 6386;
					else return 6387;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 6388;
					else return 6389;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 6390;
					else return 6391;
				else
					if (Powered) return 6392;
					else return 6393;
		}
		BlockState SpruceButton();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace SpruceDoor
	{
		enum class Half
		{
			Upper,
			Lower
		};
		enum class Hinge
		{
			Left,
			Right
		};
		constexpr BlockState SpruceDoor(const eBlockFace Facing, const enum Half Half, const enum Hinge Hinge, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8738;
							else return 8739;
						else
							if (Powered) return 8740;
							else return 8741;
					else
						if (Open)
							if (Powered) return 8742;
							else return 8743;
						else
							if (Powered) return 8744;
							else return 8745;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8746;
							else return 8747;
						else
							if (Powered) return 8748;
							else return 8749;
					else
						if (Open)
							if (Powered) return 8750;
							else return 8751;
						else
							if (Powered) return 8752;
							else return 8753;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8754;
							else return 8755;
						else
							if (Powered) return 8756;
							else return 8757;
					else
						if (Open)
							if (Powered) return 8758;
							else return 8759;
						else
							if (Powered) return 8760;
							else return 8761;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8762;
							else return 8763;
						else
							if (Powered) return 8764;
							else return 8765;
					else
						if (Open)
							if (Powered) return 8766;
							else return 8767;
						else
							if (Powered) return 8768;
							else return 8769;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8770;
							else return 8771;
						else
							if (Powered) return 8772;
							else return 8773;
					else
						if (Open)
							if (Powered) return 8774;
							else return 8775;
						else
							if (Powered) return 8776;
							else return 8777;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8778;
							else return 8779;
						else
							if (Powered) return 8780;
							else return 8781;
					else
						if (Open)
							if (Powered) return 8782;
							else return 8783;
						else
							if (Powered) return 8784;
							else return 8785;
			else
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8786;
							else return 8787;
						else
							if (Powered) return 8788;
							else return 8789;
					else
						if (Open)
							if (Powered) return 8790;
							else return 8791;
						else
							if (Powered) return 8792;
							else return 8793;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 8794;
							else return 8795;
						else
							if (Powered) return 8796;
							else return 8797;
					else
						if (Open)
							if (Powered) return 8798;
							else return 8799;
						else
							if (Powered) return 8800;
							else return 8801;
		}
		BlockState SpruceDoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Hinge Hinge(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace SpruceFence
	{
		constexpr BlockState SpruceFence(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 8580;
						else return 8581;
					else
						if (West) return 8584;
						else return 8585;
				else
					if (South)
						if (West) return 8588;
						else return 8589;
					else
						if (West) return 8592;
						else return 8593;
			else
				if (North)
					if (South)
						if (West) return 8596;
						else return 8597;
					else
						if (West) return 8600;
						else return 8601;
				else
					if (South)
						if (West) return 8604;
						else return 8605;
					else
						if (West) return 8608;
						else return 8609;
		}
		BlockState SpruceFence();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace SpruceFenceGate
	{
		constexpr BlockState SpruceFenceGate(const eBlockFace Facing, const bool InWall, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (InWall)
					if (Open)
						if (Powered) return 8418;
						else return 8419;
					else
						if (Powered) return 8420;
						else return 8421;
				else
					if (Open)
						if (Powered) return 8422;
						else return 8423;
					else
						if (Powered) return 8424;
						else return 8425;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (InWall)
					if (Open)
						if (Powered) return 8426;
						else return 8427;
					else
						if (Powered) return 8428;
						else return 8429;
				else
					if (Open)
						if (Powered) return 8430;
						else return 8431;
					else
						if (Powered) return 8432;
						else return 8433;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (InWall)
					if (Open)
						if (Powered) return 8434;
						else return 8435;
					else
						if (Powered) return 8436;
						else return 8437;
				else
					if (Open)
						if (Powered) return 8438;
						else return 8439;
					else
						if (Powered) return 8440;
						else return 8441;
			else
				if (InWall)
					if (Open)
						if (Powered) return 8442;
						else return 8443;
					else
						if (Powered) return 8444;
						else return 8445;
				else
					if (Open)
						if (Powered) return 8446;
						else return 8447;
					else
						if (Powered) return 8448;
						else return 8449;
		}
		BlockState SpruceFenceGate();
		eBlockFace Facing(BlockState Block);
		bool InWall(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace SpruceLeaves
	{
		constexpr BlockState SpruceLeaves(const unsigned char Distance, const bool Persistent)
		{
			if (Distance == 1)
				if (Persistent) return 159;
				else return 160;
			else if (Distance == 2)
				if (Persistent) return 161;
				else return 162;
			else if (Distance == 3)
				if (Persistent) return 163;
				else return 164;
			else if (Distance == 4)
				if (Persistent) return 165;
				else return 166;
			else if (Distance == 5)
				if (Persistent) return 167;
				else return 168;
			else if (Distance == 6)
				if (Persistent) return 169;
				else return 170;
			else
				if (Persistent) return 171;
				else return 172;
		}
		BlockState SpruceLeaves();
		unsigned char Distance(BlockState Block);
		bool Persistent(BlockState Block);
	}
	namespace SpruceLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState SpruceLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 76;
			else if (Axis == Axis::Y) return 77;
			else return 78;
		}
		BlockState SpruceLog();
		enum Axis Axis(BlockState Block);
	}
	namespace SprucePlanks
	{
		constexpr BlockState SprucePlanks()
		{
			return 16;
		}
	}
	namespace SprucePressurePlate
	{
		constexpr BlockState SprucePressurePlate(const bool Powered)
		{
			if (Powered) return 3875;
			else return 3876;
		}
		BlockState SprucePressurePlate();
		bool Powered(BlockState Block);
	}
	namespace SpruceSapling
	{
		constexpr BlockState SpruceSapling(const unsigned char Stage)
		{
			if (Stage == 0) return 23;
			else return 24;
		}
		BlockState SpruceSapling();
		unsigned char Stage(BlockState Block);
	}
	namespace SpruceSign
	{
		constexpr BlockState SpruceSign(const unsigned char Rotation)
		{
			if (Rotation == 0) return 3414;
			else if (Rotation == 1) return 3416;
			else if (Rotation == 2) return 3418;
			else if (Rotation == 3) return 3420;
			else if (Rotation == 4) return 3422;
			else if (Rotation == 5) return 3424;
			else if (Rotation == 6) return 3426;
			else if (Rotation == 7) return 3428;
			else if (Rotation == 8) return 3430;
			else if (Rotation == 9) return 3432;
			else if (Rotation == 10) return 3434;
			else if (Rotation == 11) return 3436;
			else if (Rotation == 12) return 3438;
			else if (Rotation == 13) return 3440;
			else if (Rotation == 14) return 3442;
			else return 3444;
		}
		BlockState SpruceSign();
		unsigned char Rotation(BlockState Block);
	}
	namespace SpruceSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState SpruceSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8307;
			else if (Type == Type::Bottom) return 8309;
			else return 8311;
		}
		BlockState SpruceSlab();
		enum Type Type(BlockState Block);
	}
	namespace SpruceStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState SpruceStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5405;
					else if (Shape == Shape::InnerLeft) return 5407;
					else if (Shape == Shape::InnerRight) return 5409;
					else if (Shape == Shape::OuterLeft) return 5411;
					else return 5413;
				else
					if (Shape == Shape::Straight) return 5415;
					else if (Shape == Shape::InnerLeft) return 5417;
					else if (Shape == Shape::InnerRight) return 5419;
					else if (Shape == Shape::OuterLeft) return 5421;
					else return 5423;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5425;
					else if (Shape == Shape::InnerLeft) return 5427;
					else if (Shape == Shape::InnerRight) return 5429;
					else if (Shape == Shape::OuterLeft) return 5431;
					else return 5433;
				else
					if (Shape == Shape::Straight) return 5435;
					else if (Shape == Shape::InnerLeft) return 5437;
					else if (Shape == Shape::InnerRight) return 5439;
					else if (Shape == Shape::OuterLeft) return 5441;
					else return 5443;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5445;
					else if (Shape == Shape::InnerLeft) return 5447;
					else if (Shape == Shape::InnerRight) return 5449;
					else if (Shape == Shape::OuterLeft) return 5451;
					else return 5453;
				else
					if (Shape == Shape::Straight) return 5455;
					else if (Shape == Shape::InnerLeft) return 5457;
					else if (Shape == Shape::InnerRight) return 5459;
					else if (Shape == Shape::OuterLeft) return 5461;
					else return 5463;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 5465;
					else if (Shape == Shape::InnerLeft) return 5467;
					else if (Shape == Shape::InnerRight) return 5469;
					else if (Shape == Shape::OuterLeft) return 5471;
					else return 5473;
				else
					if (Shape == Shape::Straight) return 5475;
					else if (Shape == Shape::InnerLeft) return 5477;
					else if (Shape == Shape::InnerRight) return 5479;
					else if (Shape == Shape::OuterLeft) return 5481;
					else return 5483;
		}
		BlockState SpruceStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace SpruceTrapdoor
	{
		enum class Half
		{
			Top,
			Bottom
		};
		constexpr BlockState SpruceTrapdoor(const eBlockFace Facing, const enum Half Half, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4176;
						else return 4178;
					else
						if (Powered) return 4180;
						else return 4182;
				else
					if (Open)
						if (Powered) return 4184;
						else return 4186;
					else
						if (Powered) return 4188;
						else return 4190;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4192;
						else return 4194;
					else
						if (Powered) return 4196;
						else return 4198;
				else
					if (Open)
						if (Powered) return 4200;
						else return 4202;
					else
						if (Powered) return 4204;
						else return 4206;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4208;
						else return 4210;
					else
						if (Powered) return 4212;
						else return 4214;
				else
					if (Open)
						if (Powered) return 4216;
						else return 4218;
					else
						if (Powered) return 4220;
						else return 4222;
			else
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 4224;
						else return 4226;
					else
						if (Powered) return 4228;
						else return 4230;
				else
					if (Open)
						if (Powered) return 4232;
						else return 4234;
					else
						if (Powered) return 4236;
						else return 4238;
		}
		BlockState SpruceTrapdoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace SpruceWallSign
	{
		constexpr BlockState SpruceWallSign(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 3744;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 3746;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 3748;
			else return 3750;
		}
		BlockState SpruceWallSign();
		eBlockFace Facing(BlockState Block);
	}
	namespace SpruceWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState SpruceWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 112;
			else if (Axis == Axis::Y) return 113;
			else return 114;
		}
		BlockState SpruceWood();
		enum Axis Axis(BlockState Block);
	}
	namespace StickyPiston
	{
		constexpr BlockState StickyPiston(const bool Extended, const eBlockFace Facing)
		{
			if (Extended)
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 1329;
				else if (Facing == eBlockFace::BLOCK_FACE_XP) return 1330;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 1331;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 1332;
				else if (Facing == eBlockFace::BLOCK_FACE_YP) return 1333;
				else return 1334;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM) return 1335;
				else if (Facing == eBlockFace::BLOCK_FACE_XP) return 1336;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 1337;
				else if (Facing == eBlockFace::BLOCK_FACE_XM) return 1338;
				else if (Facing == eBlockFace::BLOCK_FACE_YP) return 1339;
				else return 1340;
		}
		BlockState StickyPiston();
		bool Extended(BlockState Block);
		eBlockFace Facing(BlockState Block);
	}
	namespace Stone
	{
		constexpr BlockState Stone()
		{
			return 1;
		}
	}
	namespace StoneBrickSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState StoneBrickSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8379;
			else if (Type == Type::Bottom) return 8381;
			else return 8383;
		}
		BlockState StoneBrickSlab();
		enum Type Type(BlockState Block);
	}
	namespace StoneBrickStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState StoneBrickStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 4933;
					else if (Shape == Shape::InnerLeft) return 4935;
					else if (Shape == Shape::InnerRight) return 4937;
					else if (Shape == Shape::OuterLeft) return 4939;
					else return 4941;
				else
					if (Shape == Shape::Straight) return 4943;
					else if (Shape == Shape::InnerLeft) return 4945;
					else if (Shape == Shape::InnerRight) return 4947;
					else if (Shape == Shape::OuterLeft) return 4949;
					else return 4951;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 4953;
					else if (Shape == Shape::InnerLeft) return 4955;
					else if (Shape == Shape::InnerRight) return 4957;
					else if (Shape == Shape::OuterLeft) return 4959;
					else return 4961;
				else
					if (Shape == Shape::Straight) return 4963;
					else if (Shape == Shape::InnerLeft) return 4965;
					else if (Shape == Shape::InnerRight) return 4967;
					else if (Shape == Shape::OuterLeft) return 4969;
					else return 4971;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 4973;
					else if (Shape == Shape::InnerLeft) return 4975;
					else if (Shape == Shape::InnerRight) return 4977;
					else if (Shape == Shape::OuterLeft) return 4979;
					else return 4981;
				else
					if (Shape == Shape::Straight) return 4983;
					else if (Shape == Shape::InnerLeft) return 4985;
					else if (Shape == Shape::InnerRight) return 4987;
					else if (Shape == Shape::OuterLeft) return 4989;
					else return 4991;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 4993;
					else if (Shape == Shape::InnerLeft) return 4995;
					else if (Shape == Shape::InnerRight) return 4997;
					else if (Shape == Shape::OuterLeft) return 4999;
					else return 5001;
				else
					if (Shape == Shape::Straight) return 5003;
					else if (Shape == Shape::InnerLeft) return 5005;
					else if (Shape == Shape::InnerRight) return 5007;
					else if (Shape == Shape::OuterLeft) return 5009;
					else return 5011;
		}
		BlockState StoneBrickStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace StoneBrickWall
	{
		enum class East
		{
			None,
			Low,
			Tall
		};
		enum class North
		{
			None,
			Low,
			Tall
		};
		enum class South
		{
			None,
			Low,
			Tall
		};
		enum class West
		{
			None,
			Low,
			Tall
		};
		constexpr BlockState StoneBrickWall(const enum East East, const enum North North, const enum South South, const bool Up, const enum West West)
		{
			if (East == East::None)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12490;
							else if (West == West::Low) return 12491;
							else return 12492;
						else
							if (West == West::None) return 12496;
							else if (West == West::Low) return 12497;
							else return 12498;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12502;
							else if (West == West::Low) return 12503;
							else return 12504;
						else
							if (West == West::None) return 12508;
							else if (West == West::Low) return 12509;
							else return 12510;
					else
						if (Up)
							if (West == West::None) return 12514;
							else if (West == West::Low) return 12515;
							else return 12516;
						else
							if (West == West::None) return 12520;
							else if (West == West::Low) return 12521;
							else return 12522;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12526;
							else if (West == West::Low) return 12527;
							else return 12528;
						else
							if (West == West::None) return 12532;
							else if (West == West::Low) return 12533;
							else return 12534;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12538;
							else if (West == West::Low) return 12539;
							else return 12540;
						else
							if (West == West::None) return 12544;
							else if (West == West::Low) return 12545;
							else return 12546;
					else
						if (Up)
							if (West == West::None) return 12550;
							else if (West == West::Low) return 12551;
							else return 12552;
						else
							if (West == West::None) return 12556;
							else if (West == West::Low) return 12557;
							else return 12558;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12562;
							else if (West == West::Low) return 12563;
							else return 12564;
						else
							if (West == West::None) return 12568;
							else if (West == West::Low) return 12569;
							else return 12570;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12574;
							else if (West == West::Low) return 12575;
							else return 12576;
						else
							if (West == West::None) return 12580;
							else if (West == West::Low) return 12581;
							else return 12582;
					else
						if (Up)
							if (West == West::None) return 12586;
							else if (West == West::Low) return 12587;
							else return 12588;
						else
							if (West == West::None) return 12592;
							else if (West == West::Low) return 12593;
							else return 12594;
			else if (East == East::Low)
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12598;
							else if (West == West::Low) return 12599;
							else return 12600;
						else
							if (West == West::None) return 12604;
							else if (West == West::Low) return 12605;
							else return 12606;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12610;
							else if (West == West::Low) return 12611;
							else return 12612;
						else
							if (West == West::None) return 12616;
							else if (West == West::Low) return 12617;
							else return 12618;
					else
						if (Up)
							if (West == West::None) return 12622;
							else if (West == West::Low) return 12623;
							else return 12624;
						else
							if (West == West::None) return 12628;
							else if (West == West::Low) return 12629;
							else return 12630;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12634;
							else if (West == West::Low) return 12635;
							else return 12636;
						else
							if (West == West::None) return 12640;
							else if (West == West::Low) return 12641;
							else return 12642;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12646;
							else if (West == West::Low) return 12647;
							else return 12648;
						else
							if (West == West::None) return 12652;
							else if (West == West::Low) return 12653;
							else return 12654;
					else
						if (Up)
							if (West == West::None) return 12658;
							else if (West == West::Low) return 12659;
							else return 12660;
						else
							if (West == West::None) return 12664;
							else if (West == West::Low) return 12665;
							else return 12666;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12670;
							else if (West == West::Low) return 12671;
							else return 12672;
						else
							if (West == West::None) return 12676;
							else if (West == West::Low) return 12677;
							else return 12678;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12682;
							else if (West == West::Low) return 12683;
							else return 12684;
						else
							if (West == West::None) return 12688;
							else if (West == West::Low) return 12689;
							else return 12690;
					else
						if (Up)
							if (West == West::None) return 12694;
							else if (West == West::Low) return 12695;
							else return 12696;
						else
							if (West == West::None) return 12700;
							else if (West == West::Low) return 12701;
							else return 12702;
			else
				if (North == North::None)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12706;
							else if (West == West::Low) return 12707;
							else return 12708;
						else
							if (West == West::None) return 12712;
							else if (West == West::Low) return 12713;
							else return 12714;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12718;
							else if (West == West::Low) return 12719;
							else return 12720;
						else
							if (West == West::None) return 12724;
							else if (West == West::Low) return 12725;
							else return 12726;
					else
						if (Up)
							if (West == West::None) return 12730;
							else if (West == West::Low) return 12731;
							else return 12732;
						else
							if (West == West::None) return 12736;
							else if (West == West::Low) return 12737;
							else return 12738;
				else if (North == North::Low)
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12742;
							else if (West == West::Low) return 12743;
							else return 12744;
						else
							if (West == West::None) return 12748;
							else if (West == West::Low) return 12749;
							else return 12750;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12754;
							else if (West == West::Low) return 12755;
							else return 12756;
						else
							if (West == West::None) return 12760;
							else if (West == West::Low) return 12761;
							else return 12762;
					else
						if (Up)
							if (West == West::None) return 12766;
							else if (West == West::Low) return 12767;
							else return 12768;
						else
							if (West == West::None) return 12772;
							else if (West == West::Low) return 12773;
							else return 12774;
				else
					if (South == South::None)
						if (Up)
							if (West == West::None) return 12778;
							else if (West == West::Low) return 12779;
							else return 12780;
						else
							if (West == West::None) return 12784;
							else if (West == West::Low) return 12785;
							else return 12786;
					else if (South == South::Low)
						if (Up)
							if (West == West::None) return 12790;
							else if (West == West::Low) return 12791;
							else return 12792;
						else
							if (West == West::None) return 12796;
							else if (West == West::Low) return 12797;
							else return 12798;
					else
						if (Up)
							if (West == West::None) return 12802;
							else if (West == West::Low) return 12803;
							else return 12804;
						else
							if (West == West::None) return 12808;
							else if (West == West::Low) return 12809;
							else return 12810;
		}
		BlockState StoneBrickWall();
		enum East East(BlockState Block);
		enum North North(BlockState Block);
		enum South South(BlockState Block);
		bool Up(BlockState Block);
		enum West West(BlockState Block);
	}
	namespace StoneBricks
	{
		constexpr BlockState StoneBricks()
		{
			return 4495;
		}
	}
	namespace StoneButton
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState StoneButton(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 3897;
					else return 3898;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 3899;
					else return 3900;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 3901;
					else return 3902;
				else
					if (Powered) return 3903;
					else return 3904;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 3905;
					else return 3906;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 3907;
					else return 3908;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 3909;
					else return 3910;
				else
					if (Powered) return 3911;
					else return 3912;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 3913;
					else return 3914;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 3915;
					else return 3916;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 3917;
					else return 3918;
				else
					if (Powered) return 3919;
					else return 3920;
		}
		BlockState StoneButton();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace StonePressurePlate
	{
		constexpr BlockState StonePressurePlate(const bool Powered)
		{
			if (Powered) return 3807;
			else return 3808;
		}
		BlockState StonePressurePlate();
		bool Powered(BlockState Block);
	}
	namespace StoneSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState StoneSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 8337;
			else if (Type == Type::Bottom) return 8339;
			else return 8341;
		}
		BlockState StoneSlab();
		enum Type Type(BlockState Block);
	}
	namespace StoneStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState StoneStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10150;
					else if (Shape == Shape::InnerLeft) return 10152;
					else if (Shape == Shape::InnerRight) return 10154;
					else if (Shape == Shape::OuterLeft) return 10156;
					else return 10158;
				else
					if (Shape == Shape::Straight) return 10160;
					else if (Shape == Shape::InnerLeft) return 10162;
					else if (Shape == Shape::InnerRight) return 10164;
					else if (Shape == Shape::OuterLeft) return 10166;
					else return 10168;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10170;
					else if (Shape == Shape::InnerLeft) return 10172;
					else if (Shape == Shape::InnerRight) return 10174;
					else if (Shape == Shape::OuterLeft) return 10176;
					else return 10178;
				else
					if (Shape == Shape::Straight) return 10180;
					else if (Shape == Shape::InnerLeft) return 10182;
					else if (Shape == Shape::InnerRight) return 10184;
					else if (Shape == Shape::OuterLeft) return 10186;
					else return 10188;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10190;
					else if (Shape == Shape::InnerLeft) return 10192;
					else if (Shape == Shape::InnerRight) return 10194;
					else if (Shape == Shape::OuterLeft) return 10196;
					else return 10198;
				else
					if (Shape == Shape::Straight) return 10200;
					else if (Shape == Shape::InnerLeft) return 10202;
					else if (Shape == Shape::InnerRight) return 10204;
					else if (Shape == Shape::OuterLeft) return 10206;
					else return 10208;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 10210;
					else if (Shape == Shape::InnerLeft) return 10212;
					else if (Shape == Shape::InnerRight) return 10214;
					else if (Shape == Shape::OuterLeft) return 10216;
					else return 10218;
				else
					if (Shape == Shape::Straight) return 10220;
					else if (Shape == Shape::InnerLeft) return 10222;
					else if (Shape == Shape::InnerRight) return 10224;
					else if (Shape == Shape::OuterLeft) return 10226;
					else return 10228;
		}
		BlockState StoneStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace Stonecutter
	{
		constexpr BlockState Stonecutter(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 14850;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 14851;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 14852;
			else return 14853;
		}
		BlockState Stonecutter();
		eBlockFace Facing(BlockState Block);
	}
	namespace StrippedAcaciaLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedAcaciaLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 100;
			else if (Axis == Axis::Y) return 101;
			else return 102;
		}
		BlockState StrippedAcaciaLog();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedAcaciaWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedAcaciaWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 139;
			else if (Axis == Axis::Y) return 140;
			else return 141;
		}
		BlockState StrippedAcaciaWood();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedBirchLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedBirchLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 94;
			else if (Axis == Axis::Y) return 95;
			else return 96;
		}
		BlockState StrippedBirchLog();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedBirchWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedBirchWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 133;
			else if (Axis == Axis::Y) return 134;
			else return 135;
		}
		BlockState StrippedBirchWood();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedCrimsonHyphae
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedCrimsonHyphae(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 14984;
			else if (Axis == Axis::Y) return 14985;
			else return 14986;
		}
		BlockState StrippedCrimsonHyphae();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedCrimsonStem
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedCrimsonStem(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 14978;
			else if (Axis == Axis::Y) return 14979;
			else return 14980;
		}
		BlockState StrippedCrimsonStem();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedDarkOakLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedDarkOakLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 103;
			else if (Axis == Axis::Y) return 104;
			else return 105;
		}
		BlockState StrippedDarkOakLog();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedDarkOakWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedDarkOakWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 142;
			else if (Axis == Axis::Y) return 143;
			else return 144;
		}
		BlockState StrippedDarkOakWood();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedJungleLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedJungleLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 97;
			else if (Axis == Axis::Y) return 98;
			else return 99;
		}
		BlockState StrippedJungleLog();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedJungleWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedJungleWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 136;
			else if (Axis == Axis::Y) return 137;
			else return 138;
		}
		BlockState StrippedJungleWood();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedOakLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedOakLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 106;
			else if (Axis == Axis::Y) return 107;
			else return 108;
		}
		BlockState StrippedOakLog();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedOakWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedOakWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 127;
			else if (Axis == Axis::Y) return 128;
			else return 129;
		}
		BlockState StrippedOakWood();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedSpruceLog
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedSpruceLog(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 91;
			else if (Axis == Axis::Y) return 92;
			else return 93;
		}
		BlockState StrippedSpruceLog();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedSpruceWood
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedSpruceWood(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 130;
			else if (Axis == Axis::Y) return 131;
			else return 132;
		}
		BlockState StrippedSpruceWood();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedWarpedHyphae
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedWarpedHyphae(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 14967;
			else if (Axis == Axis::Y) return 14968;
			else return 14969;
		}
		BlockState StrippedWarpedHyphae();
		enum Axis Axis(BlockState Block);
	}
	namespace StrippedWarpedStem
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState StrippedWarpedStem(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 14961;
			else if (Axis == Axis::Y) return 14962;
			else return 14963;
		}
		BlockState StrippedWarpedStem();
		enum Axis Axis(BlockState Block);
	}
	namespace StructureBlock
	{
		enum class Mode
		{
			Save,
			Load,
			Corner,
			Data
		};
		constexpr BlockState StructureBlock(const enum Mode Mode)
		{
			if (Mode == Mode::Save) return 15735;
			else if (Mode == Mode::Load) return 15736;
			else if (Mode == Mode::Corner) return 15737;
			else return 15738;
		}
		BlockState StructureBlock();
		enum Mode Mode(BlockState Block);
	}
	namespace StructureVoid
	{
		constexpr BlockState StructureVoid()
		{
			return 9259;
		}
	}
	namespace SugarCane
	{
		constexpr BlockState SugarCane(const unsigned char Age)
		{
			if (Age == 0) return 3948;
			else if (Age == 1) return 3949;
			else if (Age == 2) return 3950;
			else if (Age == 3) return 3951;
			else if (Age == 4) return 3952;
			else if (Age == 5) return 3953;
			else if (Age == 6) return 3954;
			else if (Age == 7) return 3955;
			else if (Age == 8) return 3956;
			else if (Age == 9) return 3957;
			else if (Age == 10) return 3958;
			else if (Age == 11) return 3959;
			else if (Age == 12) return 3960;
			else if (Age == 13) return 3961;
			else if (Age == 14) return 3962;
			else return 3963;
		}
		BlockState SugarCane();
		unsigned char Age(BlockState Block);
	}
	namespace Sunflower
	{
		enum class Half
		{
			Upper,
			Lower
		};
		constexpr BlockState Sunflower(const enum Half Half)
		{
			if (Half == Half::Upper) return 7885;
			else return 7886;
		}
		BlockState Sunflower();
		enum Half Half(BlockState Block);
	}
	namespace SweetBerryBush
	{
		constexpr BlockState SweetBerryBush(const unsigned char Age)
		{
			if (Age == 0) return 14954;
			else if (Age == 1) return 14955;
			else if (Age == 2) return 14956;
			else return 14957;
		}
		BlockState SweetBerryBush();
		unsigned char Age(BlockState Block);
	}
	namespace TNT
	{
		constexpr BlockState TNT(const bool Unstable)
		{
			if (Unstable) return 1430;
			else return 1431;
		}
		BlockState TNT();
		bool Unstable(BlockState Block);
	}
	namespace TallGrass
	{
		enum class Half
		{
			Upper,
			Lower
		};
		constexpr BlockState TallGrass(const enum Half Half)
		{
			if (Half == Half::Upper) return 7893;
			else return 7894;
		}
		BlockState TallGrass();
		enum Half Half(BlockState Block);
	}
	namespace TallSeagrass
	{
		enum class Half
		{
			Upper,
			Lower
		};
		constexpr BlockState TallSeagrass(const enum Half Half)
		{
			if (Half == Half::Upper) return 1346;
			else return 1347;
		}
		BlockState TallSeagrass();
		enum Half Half(BlockState Block);
	}
	namespace Target
	{
		constexpr BlockState Target(const unsigned char Power)
		{
			if (Power == 0) return 15760;
			else if (Power == 1) return 15761;
			else if (Power == 2) return 15762;
			else if (Power == 3) return 15763;
			else if (Power == 4) return 15764;
			else if (Power == 5) return 15765;
			else if (Power == 6) return 15766;
			else if (Power == 7) return 15767;
			else if (Power == 8) return 15768;
			else if (Power == 9) return 15769;
			else if (Power == 10) return 15770;
			else if (Power == 11) return 15771;
			else if (Power == 12) return 15772;
			else if (Power == 13) return 15773;
			else if (Power == 14) return 15774;
			else return 15775;
		}
		BlockState Target();
		unsigned char Power(BlockState Block);
	}
	namespace Terracotta
	{
		constexpr BlockState Terracotta()
		{
			return 7882;
		}
	}
	namespace Torch
	{
		constexpr BlockState Torch()
		{
			return 1435;
		}
	}
	namespace TrappedChest
	{
		enum class Type
		{
			Single,
			Left,
			Right
		};
		constexpr BlockState TrappedChest(const eBlockFace Facing, const enum Type Type)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Type == Type::Single) return 6623;
				else if (Type == Type::Left) return 6625;
				else return 6627;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Type == Type::Single) return 6629;
				else if (Type == Type::Left) return 6631;
				else return 6633;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Type == Type::Single) return 6635;
				else if (Type == Type::Left) return 6637;
				else return 6639;
			else
				if (Type == Type::Single) return 6641;
				else if (Type == Type::Left) return 6643;
				else return 6645;
		}
		BlockState TrappedChest();
		eBlockFace Facing(BlockState Block);
		enum Type Type(BlockState Block);
	}
	namespace Tripwire
	{
		constexpr BlockState Tripwire(const bool Attached, const bool Disarmed, const bool East, const bool North, const bool Powered, const bool South, const bool West)
		{
			if (Attached)
				if (Disarmed)
					if (East)
						if (North)
							if (Powered)
								if (South)
									if (West) return 5275;
									else return 5276;
								else
									if (West) return 5277;
									else return 5278;
							else
								if (South)
									if (West) return 5279;
									else return 5280;
								else
									if (West) return 5281;
									else return 5282;
						else
							if (Powered)
								if (South)
									if (West) return 5283;
									else return 5284;
								else
									if (West) return 5285;
									else return 5286;
							else
								if (South)
									if (West) return 5287;
									else return 5288;
								else
									if (West) return 5289;
									else return 5290;
					else
						if (North)
							if (Powered)
								if (South)
									if (West) return 5291;
									else return 5292;
								else
									if (West) return 5293;
									else return 5294;
							else
								if (South)
									if (West) return 5295;
									else return 5296;
								else
									if (West) return 5297;
									else return 5298;
						else
							if (Powered)
								if (South)
									if (West) return 5299;
									else return 5300;
								else
									if (West) return 5301;
									else return 5302;
							else
								if (South)
									if (West) return 5303;
									else return 5304;
								else
									if (West) return 5305;
									else return 5306;
				else
					if (East)
						if (North)
							if (Powered)
								if (South)
									if (West) return 5307;
									else return 5308;
								else
									if (West) return 5309;
									else return 5310;
							else
								if (South)
									if (West) return 5311;
									else return 5312;
								else
									if (West) return 5313;
									else return 5314;
						else
							if (Powered)
								if (South)
									if (West) return 5315;
									else return 5316;
								else
									if (West) return 5317;
									else return 5318;
							else
								if (South)
									if (West) return 5319;
									else return 5320;
								else
									if (West) return 5321;
									else return 5322;
					else
						if (North)
							if (Powered)
								if (South)
									if (West) return 5323;
									else return 5324;
								else
									if (West) return 5325;
									else return 5326;
							else
								if (South)
									if (West) return 5327;
									else return 5328;
								else
									if (West) return 5329;
									else return 5330;
						else
							if (Powered)
								if (South)
									if (West) return 5331;
									else return 5332;
								else
									if (West) return 5333;
									else return 5334;
							else
								if (South)
									if (West) return 5335;
									else return 5336;
								else
									if (West) return 5337;
									else return 5338;
			else
				if (Disarmed)
					if (East)
						if (North)
							if (Powered)
								if (South)
									if (West) return 5339;
									else return 5340;
								else
									if (West) return 5341;
									else return 5342;
							else
								if (South)
									if (West) return 5343;
									else return 5344;
								else
									if (West) return 5345;
									else return 5346;
						else
							if (Powered)
								if (South)
									if (West) return 5347;
									else return 5348;
								else
									if (West) return 5349;
									else return 5350;
							else
								if (South)
									if (West) return 5351;
									else return 5352;
								else
									if (West) return 5353;
									else return 5354;
					else
						if (North)
							if (Powered)
								if (South)
									if (West) return 5355;
									else return 5356;
								else
									if (West) return 5357;
									else return 5358;
							else
								if (South)
									if (West) return 5359;
									else return 5360;
								else
									if (West) return 5361;
									else return 5362;
						else
							if (Powered)
								if (South)
									if (West) return 5363;
									else return 5364;
								else
									if (West) return 5365;
									else return 5366;
							else
								if (South)
									if (West) return 5367;
									else return 5368;
								else
									if (West) return 5369;
									else return 5370;
				else
					if (East)
						if (North)
							if (Powered)
								if (South)
									if (West) return 5371;
									else return 5372;
								else
									if (West) return 5373;
									else return 5374;
							else
								if (South)
									if (West) return 5375;
									else return 5376;
								else
									if (West) return 5377;
									else return 5378;
						else
							if (Powered)
								if (South)
									if (West) return 5379;
									else return 5380;
								else
									if (West) return 5381;
									else return 5382;
							else
								if (South)
									if (West) return 5383;
									else return 5384;
								else
									if (West) return 5385;
									else return 5386;
					else
						if (North)
							if (Powered)
								if (South)
									if (West) return 5387;
									else return 5388;
								else
									if (West) return 5389;
									else return 5390;
							else
								if (South)
									if (West) return 5391;
									else return 5392;
								else
									if (West) return 5393;
									else return 5394;
						else
							if (Powered)
								if (South)
									if (West) return 5395;
									else return 5396;
								else
									if (West) return 5397;
									else return 5398;
							else
								if (South)
									if (West) return 5399;
									else return 5400;
								else
									if (West) return 5401;
									else return 5402;
		}
		BlockState Tripwire();
		bool Attached(BlockState Block);
		bool Disarmed(BlockState Block);
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool Powered(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace TripwireHook
	{
		constexpr BlockState TripwireHook(const bool Attached, const eBlockFace Facing, const bool Powered)
		{
			if (Attached)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 5259;
					else return 5260;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 5261;
					else return 5262;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 5263;
					else return 5264;
				else
					if (Powered) return 5265;
					else return 5266;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 5267;
					else return 5268;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 5269;
					else return 5270;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 5271;
					else return 5272;
				else
					if (Powered) return 5273;
					else return 5274;
		}
		BlockState TripwireHook();
		bool Attached(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace TubeCoral
	{
		constexpr BlockState TubeCoral()
		{
			return 9531;
		}
	}
	namespace TubeCoralBlock
	{
		constexpr BlockState TubeCoralBlock()
		{
			return 9515;
		}
	}
	namespace TubeCoralFan
	{
		constexpr BlockState TubeCoralFan()
		{
			return 9551;
		}
	}
	namespace TubeCoralWallFan
	{
		constexpr BlockState TubeCoralWallFan(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9601;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9603;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9605;
			else return 9607;
		}
		BlockState TubeCoralWallFan();
		eBlockFace Facing(BlockState Block);
	}
	namespace TurtleEgg
	{
		constexpr BlockState TurtleEgg(const unsigned char Eggs, const unsigned char Hatch)
		{
			if (Eggs == 1)
				if (Hatch == 0) return 9498;
				else if (Hatch == 1) return 9499;
				else return 9500;
			else if (Eggs == 2)
				if (Hatch == 0) return 9501;
				else if (Hatch == 1) return 9502;
				else return 9503;
			else if (Eggs == 3)
				if (Hatch == 0) return 9504;
				else if (Hatch == 1) return 9505;
				else return 9506;
			else
				if (Hatch == 0) return 9507;
				else if (Hatch == 1) return 9508;
				else return 9509;
		}
		BlockState TurtleEgg();
		unsigned char Eggs(BlockState Block);
		unsigned char Hatch(BlockState Block);
	}
	namespace TwistingVines
	{
		constexpr BlockState TwistingVines(const unsigned char Age)
		{
			if (Age == 0) return 15017;
			else if (Age == 1) return 15018;
			else if (Age == 2) return 15019;
			else if (Age == 3) return 15020;
			else if (Age == 4) return 15021;
			else if (Age == 5) return 15022;
			else if (Age == 6) return 15023;
			else if (Age == 7) return 15024;
			else if (Age == 8) return 15025;
			else if (Age == 9) return 15026;
			else if (Age == 10) return 15027;
			else if (Age == 11) return 15028;
			else if (Age == 12) return 15029;
			else if (Age == 13) return 15030;
			else if (Age == 14) return 15031;
			else if (Age == 15) return 15032;
			else if (Age == 16) return 15033;
			else if (Age == 17) return 15034;
			else if (Age == 18) return 15035;
			else if (Age == 19) return 15036;
			else if (Age == 20) return 15037;
			else if (Age == 21) return 15038;
			else if (Age == 22) return 15039;
			else if (Age == 23) return 15040;
			else if (Age == 24) return 15041;
			else return 15042;
		}
		BlockState TwistingVines();
		unsigned char Age(BlockState Block);
	}
	namespace TwistingVinesPlant
	{
		constexpr BlockState TwistingVinesPlant()
		{
			return 15043;
		}
	}
	namespace Vine
	{
		constexpr BlockState Vine(const bool East, const bool North, const bool South, const bool Up, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (Up)
							if (West) return 4788;
							else return 4789;
						else
							if (West) return 4790;
							else return 4791;
					else
						if (Up)
							if (West) return 4792;
							else return 4793;
						else
							if (West) return 4794;
							else return 4795;
				else
					if (South)
						if (Up)
							if (West) return 4796;
							else return 4797;
						else
							if (West) return 4798;
							else return 4799;
					else
						if (Up)
							if (West) return 4800;
							else return 4801;
						else
							if (West) return 4802;
							else return 4803;
			else
				if (North)
					if (South)
						if (Up)
							if (West) return 4804;
							else return 4805;
						else
							if (West) return 4806;
							else return 4807;
					else
						if (Up)
							if (West) return 4808;
							else return 4809;
						else
							if (West) return 4810;
							else return 4811;
				else
					if (South)
						if (Up)
							if (West) return 4812;
							else return 4813;
						else
							if (West) return 4814;
							else return 4815;
					else
						if (Up)
							if (West) return 4816;
							else return 4817;
						else
							if (West) return 4818;
							else return 4819;
		}
		BlockState Vine();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool Up(BlockState Block);
		bool West(BlockState Block);
	}
	namespace VoidAir
	{
		constexpr BlockState VoidAir()
		{
			return 9665;
		}
	}
	namespace WallTorch
	{
		constexpr BlockState WallTorch(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 1436;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 1437;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 1438;
			else return 1439;
		}
		BlockState WallTorch();
		eBlockFace Facing(BlockState Block);
	}
	namespace WarpedButton
	{
		enum class Face
		{
			Floor,
			Wall,
			Ceiling
		};
		constexpr BlockState WarpedButton(const enum Face Face, const eBlockFace Facing, const bool Powered)
		{
			if (Face == Face::Floor)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 15503;
					else return 15504;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 15505;
					else return 15506;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 15507;
					else return 15508;
				else
					if (Powered) return 15509;
					else return 15510;
			else if (Face == Face::Wall)
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 15511;
					else return 15512;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 15513;
					else return 15514;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 15515;
					else return 15516;
				else
					if (Powered) return 15517;
					else return 15518;
			else
				if (Facing == eBlockFace::BLOCK_FACE_ZM)
					if (Powered) return 15519;
					else return 15520;
				else if (Facing == eBlockFace::BLOCK_FACE_ZP)
					if (Powered) return 15521;
					else return 15522;
				else if (Facing == eBlockFace::BLOCK_FACE_XM)
					if (Powered) return 15523;
					else return 15524;
				else
					if (Powered) return 15525;
					else return 15526;
		}
		BlockState WarpedButton();
		enum Face Face(BlockState Block);
		eBlockFace Facing(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace WarpedDoor
	{
		enum class Half
		{
			Upper,
			Lower
		};
		enum class Hinge
		{
			Left,
			Right
		};
		constexpr BlockState WarpedDoor(const eBlockFace Facing, const enum Half Half, const enum Hinge Hinge, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15591;
							else return 15592;
						else
							if (Powered) return 15593;
							else return 15594;
					else
						if (Open)
							if (Powered) return 15595;
							else return 15596;
						else
							if (Powered) return 15597;
							else return 15598;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15599;
							else return 15600;
						else
							if (Powered) return 15601;
							else return 15602;
					else
						if (Open)
							if (Powered) return 15603;
							else return 15604;
						else
							if (Powered) return 15605;
							else return 15606;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15607;
							else return 15608;
						else
							if (Powered) return 15609;
							else return 15610;
					else
						if (Open)
							if (Powered) return 15611;
							else return 15612;
						else
							if (Powered) return 15613;
							else return 15614;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15615;
							else return 15616;
						else
							if (Powered) return 15617;
							else return 15618;
					else
						if (Open)
							if (Powered) return 15619;
							else return 15620;
						else
							if (Powered) return 15621;
							else return 15622;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15623;
							else return 15624;
						else
							if (Powered) return 15625;
							else return 15626;
					else
						if (Open)
							if (Powered) return 15627;
							else return 15628;
						else
							if (Powered) return 15629;
							else return 15630;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15631;
							else return 15632;
						else
							if (Powered) return 15633;
							else return 15634;
					else
						if (Open)
							if (Powered) return 15635;
							else return 15636;
						else
							if (Powered) return 15637;
							else return 15638;
			else
				if (Half == Half::Upper)
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15639;
							else return 15640;
						else
							if (Powered) return 15641;
							else return 15642;
					else
						if (Open)
							if (Powered) return 15643;
							else return 15644;
						else
							if (Powered) return 15645;
							else return 15646;
				else
					if (Hinge == Hinge::Left)
						if (Open)
							if (Powered) return 15647;
							else return 15648;
						else
							if (Powered) return 15649;
							else return 15650;
					else
						if (Open)
							if (Powered) return 15651;
							else return 15652;
						else
							if (Powered) return 15653;
							else return 15654;
		}
		BlockState WarpedDoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Hinge Hinge(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace WarpedFence
	{
		constexpr BlockState WarpedFence(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 15097;
						else return 15098;
					else
						if (West) return 15101;
						else return 15102;
				else
					if (South)
						if (West) return 15105;
						else return 15106;
					else
						if (West) return 15109;
						else return 15110;
			else
				if (North)
					if (South)
						if (West) return 15113;
						else return 15114;
					else
						if (West) return 15117;
						else return 15118;
				else
					if (South)
						if (West) return 15121;
						else return 15122;
					else
						if (West) return 15125;
						else return 15126;
		}
		BlockState WarpedFence();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace WarpedFenceGate
	{
		constexpr BlockState WarpedFenceGate(const eBlockFace Facing, const bool InWall, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (InWall)
					if (Open)
						if (Powered) return 15287;
						else return 15288;
					else
						if (Powered) return 15289;
						else return 15290;
				else
					if (Open)
						if (Powered) return 15291;
						else return 15292;
					else
						if (Powered) return 15293;
						else return 15294;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (InWall)
					if (Open)
						if (Powered) return 15295;
						else return 15296;
					else
						if (Powered) return 15297;
						else return 15298;
				else
					if (Open)
						if (Powered) return 15299;
						else return 15300;
					else
						if (Powered) return 15301;
						else return 15302;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (InWall)
					if (Open)
						if (Powered) return 15303;
						else return 15304;
					else
						if (Powered) return 15305;
						else return 15306;
				else
					if (Open)
						if (Powered) return 15307;
						else return 15308;
					else
						if (Powered) return 15309;
						else return 15310;
			else
				if (InWall)
					if (Open)
						if (Powered) return 15311;
						else return 15312;
					else
						if (Powered) return 15313;
						else return 15314;
				else
					if (Open)
						if (Powered) return 15315;
						else return 15316;
					else
						if (Powered) return 15317;
						else return 15318;
		}
		BlockState WarpedFenceGate();
		eBlockFace Facing(BlockState Block);
		bool InWall(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace WarpedFungus
	{
		constexpr BlockState WarpedFungus()
		{
			return 14971;
		}
	}
	namespace WarpedHyphae
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState WarpedHyphae(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 14964;
			else if (Axis == Axis::Y) return 14965;
			else return 14966;
		}
		BlockState WarpedHyphae();
		enum Axis Axis(BlockState Block);
	}
	namespace WarpedNylium
	{
		constexpr BlockState WarpedNylium()
		{
			return 14970;
		}
	}
	namespace WarpedPlanks
	{
		constexpr BlockState WarpedPlanks()
		{
			return 15046;
		}
	}
	namespace WarpedPressurePlate
	{
		constexpr BlockState WarpedPressurePlate(const bool Powered)
		{
			if (Powered) return 15061;
			else return 15062;
		}
		BlockState WarpedPressurePlate();
		bool Powered(BlockState Block);
	}
	namespace WarpedRoots
	{
		constexpr BlockState WarpedRoots()
		{
			return 14973;
		}
	}
	namespace WarpedSign
	{
		constexpr BlockState WarpedSign(const unsigned char Rotation)
		{
			if (Rotation == 0) return 15688;
			else if (Rotation == 1) return 15690;
			else if (Rotation == 2) return 15692;
			else if (Rotation == 3) return 15694;
			else if (Rotation == 4) return 15696;
			else if (Rotation == 5) return 15698;
			else if (Rotation == 6) return 15700;
			else if (Rotation == 7) return 15702;
			else if (Rotation == 8) return 15704;
			else if (Rotation == 9) return 15706;
			else if (Rotation == 10) return 15708;
			else if (Rotation == 11) return 15710;
			else if (Rotation == 12) return 15712;
			else if (Rotation == 13) return 15714;
			else if (Rotation == 14) return 15716;
			else return 15718;
		}
		BlockState WarpedSign();
		unsigned char Rotation(BlockState Block);
	}
	namespace WarpedSlab
	{
		enum class Type
		{
			Top,
			Bottom,
			Double
		};
		constexpr BlockState WarpedSlab(const enum Type Type)
		{
			if (Type == Type::Top) return 15054;
			else if (Type == Type::Bottom) return 15056;
			else return 15058;
		}
		BlockState WarpedSlab();
		enum Type Type(BlockState Block);
	}
	namespace WarpedStairs
	{
		enum class Half
		{
			Top,
			Bottom
		};
		enum class Shape
		{
			Straight,
			InnerLeft,
			InnerRight,
			OuterLeft,
			OuterRight
		};
		constexpr BlockState WarpedStairs(const eBlockFace Facing, const enum Half Half, const enum Shape Shape)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15400;
					else if (Shape == Shape::InnerLeft) return 15402;
					else if (Shape == Shape::InnerRight) return 15404;
					else if (Shape == Shape::OuterLeft) return 15406;
					else return 15408;
				else
					if (Shape == Shape::Straight) return 15410;
					else if (Shape == Shape::InnerLeft) return 15412;
					else if (Shape == Shape::InnerRight) return 15414;
					else if (Shape == Shape::OuterLeft) return 15416;
					else return 15418;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15420;
					else if (Shape == Shape::InnerLeft) return 15422;
					else if (Shape == Shape::InnerRight) return 15424;
					else if (Shape == Shape::OuterLeft) return 15426;
					else return 15428;
				else
					if (Shape == Shape::Straight) return 15430;
					else if (Shape == Shape::InnerLeft) return 15432;
					else if (Shape == Shape::InnerRight) return 15434;
					else if (Shape == Shape::OuterLeft) return 15436;
					else return 15438;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15440;
					else if (Shape == Shape::InnerLeft) return 15442;
					else if (Shape == Shape::InnerRight) return 15444;
					else if (Shape == Shape::OuterLeft) return 15446;
					else return 15448;
				else
					if (Shape == Shape::Straight) return 15450;
					else if (Shape == Shape::InnerLeft) return 15452;
					else if (Shape == Shape::InnerRight) return 15454;
					else if (Shape == Shape::OuterLeft) return 15456;
					else return 15458;
			else
				if (Half == Half::Top)
					if (Shape == Shape::Straight) return 15460;
					else if (Shape == Shape::InnerLeft) return 15462;
					else if (Shape == Shape::InnerRight) return 15464;
					else if (Shape == Shape::OuterLeft) return 15466;
					else return 15468;
				else
					if (Shape == Shape::Straight) return 15470;
					else if (Shape == Shape::InnerLeft) return 15472;
					else if (Shape == Shape::InnerRight) return 15474;
					else if (Shape == Shape::OuterLeft) return 15476;
					else return 15478;
		}
		BlockState WarpedStairs();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		enum Shape Shape(BlockState Block);
	}
	namespace WarpedStem
	{
		enum class Axis
		{
			X,
			Y,
			Z
		};
		constexpr BlockState WarpedStem(const enum Axis Axis)
		{
			if (Axis == Axis::X) return 14958;
			else if (Axis == Axis::Y) return 14959;
			else return 14960;
		}
		BlockState WarpedStem();
		enum Axis Axis(BlockState Block);
	}
	namespace WarpedTrapdoor
	{
		enum class Half
		{
			Top,
			Bottom
		};
		constexpr BlockState WarpedTrapdoor(const eBlockFace Facing, const enum Half Half, const bool Open, const bool Powered)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 15192;
						else return 15194;
					else
						if (Powered) return 15196;
						else return 15198;
				else
					if (Open)
						if (Powered) return 15200;
						else return 15202;
					else
						if (Powered) return 15204;
						else return 15206;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 15208;
						else return 15210;
					else
						if (Powered) return 15212;
						else return 15214;
				else
					if (Open)
						if (Powered) return 15216;
						else return 15218;
					else
						if (Powered) return 15220;
						else return 15222;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 15224;
						else return 15226;
					else
						if (Powered) return 15228;
						else return 15230;
				else
					if (Open)
						if (Powered) return 15232;
						else return 15234;
					else
						if (Powered) return 15236;
						else return 15238;
			else
				if (Half == Half::Top)
					if (Open)
						if (Powered) return 15240;
						else return 15242;
					else
						if (Powered) return 15244;
						else return 15246;
				else
					if (Open)
						if (Powered) return 15248;
						else return 15250;
					else
						if (Powered) return 15252;
						else return 15254;
		}
		BlockState WarpedTrapdoor();
		eBlockFace Facing(BlockState Block);
		enum Half Half(BlockState Block);
		bool Open(BlockState Block);
		bool Powered(BlockState Block);
	}
	namespace WarpedWallSign
	{
		constexpr BlockState WarpedWallSign(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 15728;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 15730;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 15732;
			else return 15734;
		}
		BlockState WarpedWallSign();
		eBlockFace Facing(BlockState Block);
	}
	namespace WarpedWartBlock
	{
		constexpr BlockState WarpedWartBlock()
		{
			return 14972;
		}
	}
	namespace Water
	{
		constexpr BlockState Water(const unsigned char Level)
		{
			if (Level == 0) return 34;
			else if (Level == 1) return 35;
			else if (Level == 2) return 36;
			else if (Level == 3) return 37;
			else if (Level == 4) return 38;
			else if (Level == 5) return 39;
			else if (Level == 6) return 40;
			else if (Level == 7) return 41;
			else if (Level == 8) return 42;
			else if (Level == 9) return 43;
			else if (Level == 10) return 44;
			else if (Level == 11) return 45;
			else if (Level == 12) return 46;
			else if (Level == 13) return 47;
			else if (Level == 14) return 48;
			else return 49;
		}
		BlockState Water();
		unsigned char Level(BlockState Block);
	}
	namespace WeepingVines
	{
		constexpr BlockState WeepingVines(const unsigned char Age)
		{
			if (Age == 0) return 14990;
			else if (Age == 1) return 14991;
			else if (Age == 2) return 14992;
			else if (Age == 3) return 14993;
			else if (Age == 4) return 14994;
			else if (Age == 5) return 14995;
			else if (Age == 6) return 14996;
			else if (Age == 7) return 14997;
			else if (Age == 8) return 14998;
			else if (Age == 9) return 14999;
			else if (Age == 10) return 15000;
			else if (Age == 11) return 15001;
			else if (Age == 12) return 15002;
			else if (Age == 13) return 15003;
			else if (Age == 14) return 15004;
			else if (Age == 15) return 15005;
			else if (Age == 16) return 15006;
			else if (Age == 17) return 15007;
			else if (Age == 18) return 15008;
			else if (Age == 19) return 15009;
			else if (Age == 20) return 15010;
			else if (Age == 21) return 15011;
			else if (Age == 22) return 15012;
			else if (Age == 23) return 15013;
			else if (Age == 24) return 15014;
			else return 15015;
		}
		BlockState WeepingVines();
		unsigned char Age(BlockState Block);
	}
	namespace WeepingVinesPlant
	{
		constexpr BlockState WeepingVinesPlant()
		{
			return 15016;
		}
	}
	namespace WetSponge
	{
		constexpr BlockState WetSponge()
		{
			return 230;
		}
	}
	namespace Wheat
	{
		constexpr BlockState Wheat(const unsigned char Age)
		{
			if (Age == 0) return 3357;
			else if (Age == 1) return 3358;
			else if (Age == 2) return 3359;
			else if (Age == 3) return 3360;
			else if (Age == 4) return 3361;
			else if (Age == 5) return 3362;
			else if (Age == 6) return 3363;
			else return 3364;
		}
		BlockState Wheat();
		unsigned char Age(BlockState Block);
	}
	namespace WhiteBanner
	{
		constexpr BlockState WhiteBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 7897;
			else if (Rotation == 1) return 7898;
			else if (Rotation == 2) return 7899;
			else if (Rotation == 3) return 7900;
			else if (Rotation == 4) return 7901;
			else if (Rotation == 5) return 7902;
			else if (Rotation == 6) return 7903;
			else if (Rotation == 7) return 7904;
			else if (Rotation == 8) return 7905;
			else if (Rotation == 9) return 7906;
			else if (Rotation == 10) return 7907;
			else if (Rotation == 11) return 7908;
			else if (Rotation == 12) return 7909;
			else if (Rotation == 13) return 7910;
			else if (Rotation == 14) return 7911;
			else return 7912;
		}
		BlockState WhiteBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace WhiteBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState WhiteBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1049;
					else return 1050;
				else
					if (Part == Part::Head) return 1051;
					else return 1052;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1053;
					else return 1054;
				else
					if (Part == Part::Head) return 1055;
					else return 1056;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1057;
					else return 1058;
				else
					if (Part == Part::Head) return 1059;
					else return 1060;
			else
				if (Occupied)
					if (Part == Part::Head) return 1061;
					else return 1062;
				else
					if (Part == Part::Head) return 1063;
					else return 1064;
		}
		BlockState WhiteBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace WhiteCarpet
	{
		constexpr BlockState WhiteCarpet()
		{
			return 7866;
		}
	}
	namespace WhiteConcrete
	{
		constexpr BlockState WhiteConcrete()
		{
			return 9438;
		}
	}
	namespace WhiteConcretePowder
	{
		constexpr BlockState WhiteConcretePowder()
		{
			return 9454;
		}
	}
	namespace WhiteGlazedTerracotta
	{
		constexpr BlockState WhiteGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9374;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9375;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9376;
			else return 9377;
		}
		BlockState WhiteGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace WhiteShulkerBox
	{
		constexpr BlockState WhiteShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9278;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9279;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9280;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9281;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9282;
			else return 9283;
		}
		BlockState WhiteShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace WhiteStainedGlass
	{
		constexpr BlockState WhiteStainedGlass()
		{
			return 4095;
		}
	}
	namespace WhiteStainedGlassPane
	{
		constexpr BlockState WhiteStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 6865;
						else return 6866;
					else
						if (West) return 6869;
						else return 6870;
				else
					if (South)
						if (West) return 6873;
						else return 6874;
					else
						if (West) return 6877;
						else return 6878;
			else
				if (North)
					if (South)
						if (West) return 6881;
						else return 6882;
					else
						if (West) return 6885;
						else return 6886;
				else
					if (South)
						if (West) return 6889;
						else return 6890;
					else
						if (West) return 6893;
						else return 6894;
		}
		BlockState WhiteStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace WhiteTerracotta
	{
		constexpr BlockState WhiteTerracotta()
		{
			return 6847;
		}
	}
	namespace WhiteTulip
	{
		constexpr BlockState WhiteTulip()
		{
			return 1419;
		}
	}
	namespace WhiteWallBanner
	{
		constexpr BlockState WhiteWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8153;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8154;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8155;
			else return 8156;
		}
		BlockState WhiteWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace WhiteWool
	{
		constexpr BlockState WhiteWool()
		{
			return 1384;
		}
	}
	namespace WitherRose
	{
		constexpr BlockState WitherRose()
		{
			return 1423;
		}
	}
	namespace WitherSkeletonSkull
	{
		constexpr BlockState WitherSkeletonSkull(const unsigned char Rotation)
		{
			if (Rotation == 0) return 6510;
			else if (Rotation == 1) return 6511;
			else if (Rotation == 2) return 6512;
			else if (Rotation == 3) return 6513;
			else if (Rotation == 4) return 6514;
			else if (Rotation == 5) return 6515;
			else if (Rotation == 6) return 6516;
			else if (Rotation == 7) return 6517;
			else if (Rotation == 8) return 6518;
			else if (Rotation == 9) return 6519;
			else if (Rotation == 10) return 6520;
			else if (Rotation == 11) return 6521;
			else if (Rotation == 12) return 6522;
			else if (Rotation == 13) return 6523;
			else if (Rotation == 14) return 6524;
			else return 6525;
		}
		BlockState WitherSkeletonSkull();
		unsigned char Rotation(BlockState Block);
	}
	namespace WitherSkeletonWallSkull
	{
		constexpr BlockState WitherSkeletonWallSkull(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6526;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6527;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6528;
			else return 6529;
		}
		BlockState WitherSkeletonWallSkull();
		eBlockFace Facing(BlockState Block);
	}
	namespace YellowBanner
	{
		constexpr BlockState YellowBanner(const unsigned char Rotation)
		{
			if (Rotation == 0) return 7961;
			else if (Rotation == 1) return 7962;
			else if (Rotation == 2) return 7963;
			else if (Rotation == 3) return 7964;
			else if (Rotation == 4) return 7965;
			else if (Rotation == 5) return 7966;
			else if (Rotation == 6) return 7967;
			else if (Rotation == 7) return 7968;
			else if (Rotation == 8) return 7969;
			else if (Rotation == 9) return 7970;
			else if (Rotation == 10) return 7971;
			else if (Rotation == 11) return 7972;
			else if (Rotation == 12) return 7973;
			else if (Rotation == 13) return 7974;
			else if (Rotation == 14) return 7975;
			else return 7976;
		}
		BlockState YellowBanner();
		unsigned char Rotation(BlockState Block);
	}
	namespace YellowBed
	{
		enum class Part
		{
			Head,
			Foot
		};
		constexpr BlockState YellowBed(const eBlockFace Facing, const bool Occupied, const enum Part Part)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM)
				if (Occupied)
					if (Part == Part::Head) return 1113;
					else return 1114;
				else
					if (Part == Part::Head) return 1115;
					else return 1116;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP)
				if (Occupied)
					if (Part == Part::Head) return 1117;
					else return 1118;
				else
					if (Part == Part::Head) return 1119;
					else return 1120;
			else if (Facing == eBlockFace::BLOCK_FACE_XM)
				if (Occupied)
					if (Part == Part::Head) return 1121;
					else return 1122;
				else
					if (Part == Part::Head) return 1123;
					else return 1124;
			else
				if (Occupied)
					if (Part == Part::Head) return 1125;
					else return 1126;
				else
					if (Part == Part::Head) return 1127;
					else return 1128;
		}
		BlockState YellowBed();
		eBlockFace Facing(BlockState Block);
		bool Occupied(BlockState Block);
		enum Part Part(BlockState Block);
	}
	namespace YellowCarpet
	{
		constexpr BlockState YellowCarpet()
		{
			return 7870;
		}
	}
	namespace YellowConcrete
	{
		constexpr BlockState YellowConcrete()
		{
			return 9442;
		}
	}
	namespace YellowConcretePowder
	{
		constexpr BlockState YellowConcretePowder()
		{
			return 9458;
		}
	}
	namespace YellowGlazedTerracotta
	{
		constexpr BlockState YellowGlazedTerracotta(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9390;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9391;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9392;
			else return 9393;
		}
		BlockState YellowGlazedTerracotta();
		eBlockFace Facing(BlockState Block);
	}
	namespace YellowShulkerBox
	{
		constexpr BlockState YellowShulkerBox(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 9302;
			else if (Facing == eBlockFace::BLOCK_FACE_XP) return 9303;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 9304;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 9305;
			else if (Facing == eBlockFace::BLOCK_FACE_YP) return 9306;
			else return 9307;
		}
		BlockState YellowShulkerBox();
		eBlockFace Facing(BlockState Block);
	}
	namespace YellowStainedGlass
	{
		constexpr BlockState YellowStainedGlass()
		{
			return 4099;
		}
	}
	namespace YellowStainedGlassPane
	{
		constexpr BlockState YellowStainedGlassPane(const bool East, const bool North, const bool South, const bool West)
		{
			if (East)
				if (North)
					if (South)
						if (West) return 6993;
						else return 6994;
					else
						if (West) return 6997;
						else return 6998;
				else
					if (South)
						if (West) return 7001;
						else return 7002;
					else
						if (West) return 7005;
						else return 7006;
			else
				if (North)
					if (South)
						if (West) return 7009;
						else return 7010;
					else
						if (West) return 7013;
						else return 7014;
				else
					if (South)
						if (West) return 7017;
						else return 7018;
					else
						if (West) return 7021;
						else return 7022;
		}
		BlockState YellowStainedGlassPane();
		bool East(BlockState Block);
		bool North(BlockState Block);
		bool South(BlockState Block);
		bool West(BlockState Block);
	}
	namespace YellowTerracotta
	{
		constexpr BlockState YellowTerracotta()
		{
			return 6851;
		}
	}
	namespace YellowWallBanner
	{
		constexpr BlockState YellowWallBanner(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 8169;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 8170;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 8171;
			else return 8172;
		}
		BlockState YellowWallBanner();
		eBlockFace Facing(BlockState Block);
	}
	namespace YellowWool
	{
		constexpr BlockState YellowWool()
		{
			return 1388;
		}
	}
	namespace ZombieHead
	{
		constexpr BlockState ZombieHead(const unsigned char Rotation)
		{
			if (Rotation == 0) return 6530;
			else if (Rotation == 1) return 6531;
			else if (Rotation == 2) return 6532;
			else if (Rotation == 3) return 6533;
			else if (Rotation == 4) return 6534;
			else if (Rotation == 5) return 6535;
			else if (Rotation == 6) return 6536;
			else if (Rotation == 7) return 6537;
			else if (Rotation == 8) return 6538;
			else if (Rotation == 9) return 6539;
			else if (Rotation == 10) return 6540;
			else if (Rotation == 11) return 6541;
			else if (Rotation == 12) return 6542;
			else if (Rotation == 13) return 6543;
			else if (Rotation == 14) return 6544;
			else return 6545;
		}
		BlockState ZombieHead();
		unsigned char Rotation(BlockState Block);
	}
	namespace ZombieWallHead
	{
		constexpr BlockState ZombieWallHead(const eBlockFace Facing)
		{
			if (Facing == eBlockFace::BLOCK_FACE_ZM) return 6546;
			else if (Facing == eBlockFace::BLOCK_FACE_ZP) return 6547;
			else if (Facing == eBlockFace::BLOCK_FACE_XM) return 6548;
			else return 6549;
		}
		BlockState ZombieWallHead();
		eBlockFace Facing(BlockState Block);
	}
}
