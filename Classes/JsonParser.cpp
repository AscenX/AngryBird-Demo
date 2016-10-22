//
//  JsonParser.cpp
//  AngryBird
//
//  Created by Ascen on 16/10/21.
//
//

#include "JsonParser.hpp"
#include "external/json/document.h"


Vector<SpriteData*> JsonParser::getAllSpriteWithFile(std::string fileName){
    Vector<SpriteData*> spriteDataVector;
    rapidjson::Document document;
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(filePath);
    document.Parse<0>(contentStr.c_str());
    const rapidjson::Value& spriteVector = document["sprites"];
    for(int i = 0;i < spriteVector.Size();++i){
        int tag = spriteVector[i]["tag"].GetInt();
        int x = spriteVector[i]["x"].GetInt();
        int y = spriteVector[i]["y"].GetInt();
        
        SpriteData* spriteData = SpriteData::create();
        spriteData->setTag(tag);
        spriteData->setX(x);
        spriteData->setY(y);
        spriteDataVector.pushBack(spriteData);
    }
    return spriteDataVector;
}

