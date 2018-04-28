//#pragma once
//
//#include "Component.h"
//
//class TheoryNode
//{
//	Handle _subTheoryAH;
//	Handle _subTheoryBH;
//	Modifier _modifier = Modifier::None;
//	Operator _operator = Operator::LeftStream;
//
//	float _scale = 1.0f;
//	float _offset = 0.0f;
//	float _filterValue = 0.0f;
//	Handle _streamH;
//
//	float _precision = 0.0f;
//	float _recursivePrecision = 0.0f;
//
//	/*static constexpr Operator operators[] = {Operator::Addition, Operator::Multiplication};
//	static constexpr int numOperations = 2;*/
//
//	static constexpr Modifier kModifiers[] = { Modifier::Derivation, Modifier::Integration, Modifier::Filter, Modifier::None };
//	static constexpr int kNumModifiers = 4;
//
//	static constexpr Operator kOperators[] = { Operator::Addition, Operator::Multiplication, Operator::LeftStream };
//	static constexpr int kNumOperators = 3;
//
//	static constexpr int kNumTests = 16;
//
//	static void detectFilter(const float values[kNumTests], const int idxs[kNumTests], int level, const float* cause, float& filterValue,
//		float& filterOffset, float& filterScale);
//
//public:
//	static constexpr unsigned int maxSize = 1000;
//
//	void init(Modifier modifier, float scale, float offset, Handle streamH, float precision, float filterValue = 0.0f)
//	{
//		_modifier = modifier;
//		_scale = scale;
//		_offset = offset;
//		_streamH = streamH;
//		_precision = precision;
//		_filterValue = filterValue;
//		_recursivePrecision = precision;
//	};
//
//	void update(float* stream);
//
//	static Handle createTheory(float values[kNumTests], int idxs[kNumTests], int level);
//	static Handle createRandomTheory(int remainingDepth);
//};
