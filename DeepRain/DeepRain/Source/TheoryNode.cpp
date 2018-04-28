//#include "Theory.h"
//
//#include "Drawer.h"
//#include "StreamComp.h"
//#include <stdlib.h>
//
// void Theory::update(float* stream) {}
//
// Handle Theory::createTheory(float values[kNumTests], int idxs[kNumTests], int level)
//{
//    Handle theoryH;
//
//    const int kAmount = Stream::kFirstLevelSize >> level;
//    int handleIdx = 0;
//    int maxPrecisionHandleIdx = INVALID_HANDLE_IDX;
//    float maxPrecision = 0.0f;
//    float maxPrecisionScale = 1.0f;
//    float maxPrecisionOffset = 0.0f;
//    float maxPrecisionFilterValue = 0.0f;
//    Modifier maxPrecisionModifier = Modifier::None;
//
//    for (int i = 0; i < (int)Component<Stream>::aliveSize; ++i, ++handleIdx)
//    {
//        for (; Component<Stream>::handles[handleIdx].idx == INVALID_HANDLE_IDX; ++handleIdx)
//            ;
//
//        const float* cause = Component<Stream>::objects[Component<Stream>::handles[handleIdx].idx].getData();
//
//        for (int modifierIdx = 0; modifierIdx < kNumModifiers; ++modifierIdx)
//        {
//            Modifier modifier = kModifiers[modifierIdx];
//            float avgScale = 1.0f;
//            float filterOffset = 0.0f;
//            float filterScale = 0.0f;
//            float filterValue = 0.0f;
//
//            if (modifier == Modifier::Filter)
//            {
//                detectFilter(values, idxs, level, cause, filterValue, filterOffset, filterScale);
//            }
//
//            for (int scales = 0; scales < 2; ++scales)
//            {
//                float accValue = 0.0f;
//                float accError = 0.0f;
//                float scale = avgScale;
//                float offset = 0.0f;
//
//                for (int j = 0; j < kNumTests; ++j)
//                {
//                    float prediction = 0.0f;
//
//                    switch (modifier)
//                    {
//                        case Modifier::None:
//                            prediction = cause[idxs[j]] * scale;
//                            break;
//                        case Modifier::Derivation:
//                            prediction = (cause[idxs[j]] - cause[(idxs[j] + 1) % kAmount]) * scale;
//                            break;
//                        case Modifier::Integration:
//                            prediction = (values[(idxs[j] + 1) % kAmount] + cause[idxs[j]] - cause[(idxs[j] + 1) % kAmount]) * scale;
//                            break;
//                        case Modifier::Filter:
//                            prediction = filterOffset + (cause[idxs[j]] >= filterValue ? filterScale : -filterScale);
//                            scale = filterScale;
//                            offset = filterOffset;
//                            break;
//                    }
//
//                    accValue += fabs(values[j]);
//                    accError += fabs(values[j] - prediction);
//
//                    avgScale += values[j] / prediction;
//                }
//
//                float precision = (accValue - accError) / accValue;
//                avgScale /= kNumTests;
//
//                if (precision > maxPrecision)
//                {
//                    maxPrecision = precision;
//                    maxPrecisionHandleIdx = handleIdx;
//                    maxPrecisionScale = scale;
//                    maxPrecisionModifier = modifier;
//                    maxPrecisionFilterValue = filterValue;
//                    maxPrecisionOffset = offset;
//                }
//
//                if (modifier == Modifier::Filter)
//                {
//                    break;
//                }
//            }
//        }
//    }
//
//    if (maxPrecisionHandleIdx == INVALID_HANDLE_IDX)
//    {
//        return theoryH;
//    }
//
//    bool ok = Drawer::addComponent<Theory>(theoryH);
//
//    if (!ok)
//    {
//        return theoryH;
//    }
//
//    Handle streamH;
//    streamH.idx = maxPrecisionHandleIdx;
//    streamH.generation = Component<Stream>::handles[handleIdx].generation;
//
//    Drawer::getComponent<Theory>(theoryH).init(maxPrecisionModifier, maxPrecisionScale, maxPrecisionOffset, streamH, maxPrecision,
//                                               maxPrecisionFilterValue);
//
//    return theoryH;
//}
//
// void Theory::detectFilter(const float values[kNumTests], const int idxs[kNumTests], int level, const float* cause, float& filterValue,
//                          float& filterOffset, float& filterScale)
//{
//    float min = 9999999999.0f;
//    float max = -9999999999.0f;
//
//    for (int j = 0; j < kNumTests; ++j)
//    {
//        min = cause[idxs[j]] < min ? cause[idxs[j]] : min;
//        max = cause[idxs[j]] > max ? cause[idxs[j]] : max;
//    }
//
//    filterOffset = (max + min) * 0.5f;
//
//    for (int j = 0; j < kNumTests; ++j)
//    {
//        filterScale += fabs(cause[idxs[j]] - filterOffset);
//    }
//
//    filterScale /= kNumTests;
//
//    float minBelow = 9999999999.0f;
//    float maxBelow = -9999999999.0f;
//    float minOver = 9999999999.0f;
//    float maxOver = -9999999999.0f;
//
//    for (int j = 0; j < kNumTests; ++j)
//    {
//        if (cause[idxs[j]] < filterOffset)
//        {
//            minBelow = values[j] < minBelow ? values[j] : minBelow;
//            maxBelow = values[j] > maxBelow ? values[j] : maxBelow;
//        }
//        else
//        {
//            minOver = values[j] < minOver ? values[j] : minOver;
//            maxOver = values[j] > maxOver ? values[j] : maxOver;
//        }
//    }
//
//    if (maxOver > minBelow)
//    {
//        filterValue = (minOver + maxBelow) * 0.5f;
//    }
//    else
//    {
//        filterValue = (minBelow + maxOver) * 0.5f;
//        filterScale *= -1.0f;
//    }
//}
//
// Handle Theory::createRandomTheory(int remainingDepth)
//{
//    //Operator op = remainingDepth > 0 && Drawer::getNumAvailableComponents<Theory>() > 2 ? kOperators[rand() % kNumOperators] : Operator::LeftStream;
//    //Modifier modifier = Modifier::None;
//    //Handle streamH;
//    //Handle theoryH;
//
//    //Drawer::addComponent<Theory>(theoryH);
//    //assert(Drawer::isHandleValid<Theory>(theoryH));
//
//    //if (op == Operator::LeftStream)
//    //{
//    //    modifier = kModifiers[rand() % kNumModifiers];
//
//    //    int aliveHandleIdx = rand() % Component<Stream>::aliveSize;
//    //    int handleIdx = 0;
//
//    //    for (int i = 0; i < aliveHandleIdx; ++i, ++handleIdx)
//    //        for (; Component<Stream>::handles[handleIdx].idx == INVALID_HANDLE_IDX; ++handleIdx)
//    //            ;
//
//    //    streamH.idx = handleIdx;
//    //    streamH.generation = Component<Stream>::handles[handleIdx].generation;
//    //}
//    //else
//    //{
//    //    Handle subTheoryAH = createRandomTheory(remainingDepth - 1);
//    //    Handle subTheoryBH = createRandomTheory(remainingDepth - 1);
//    //}
//
//    //Drawer::getComponent<Theory>(theoryH).init(modifier, maxPrecisionScale, maxPrecisionOffset, streamH, maxPrecision,
//    //                                           maxPrecisionFilterValue);
//
//    //return theoryH;
//}
