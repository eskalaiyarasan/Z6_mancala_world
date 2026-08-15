/**
 * @file source/engine/z6_engine_singleton.cpp
 * @author  kalaiyarasan S
 * @date 2026-08-06
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 * This code and all associated intellectual property completely
 * belong to the ZERASIX GAMING STUDIO LLP.
 * Unauthorized copying, distribution, or use is strictly prohibited.
 */

#include <z6_engine_pipeline.h>
#include <z6_engine_singleton.h>
#include <z6_utils.h>
#include <root_schema.h>

 std::unique_ptr<Z6PipelineContext> QualifyStage::Execute(std::unique_ptr<Z6PipelineContext> context){

     // Cast to our extended context to access message and error flags
     auto ext_context = dynamic_cast<Z6ExtendedPipelineContext*>(context.get());
     if (!ext_context) {
         return context; // Invalid context type
     }

     // Validate message following movereq json schema[cite: 1]
     if (!Z6MoveReqJson::validate(ext_context->get_msg())) {
         ext_context->has_error = 100; // Set error code to 100 on failure
     }
     if( ! compareStr(Z6EngineSpace::config.id)){
         ext_context->has_error = 101;
     }
     int turn;
     Z6BoardJson board;
     Z6MoveJson  move;
     int side=-1;
     Z6Root& root = Z6Root::getInstance().get();
     ext_context->update_request();
     if( root.getBoard(board) && board.getTurn(turn) && ext_context->get_request().getMove(move)  ){
         std::string player;
         Z6GameJson game;
         std::vector<std::string> plist;
         if( move.getPlayer(player) && root.getGame(game)&& game.getPlayer(plist)){
            for( const auto &p : plist){
                 side ++;
                 compareStr(p , player)? break : continue;
            }
            if( turn != side){
                ext_context->has_error = 102;
            }
         }
     }

     return context;
 }
std::unique_ptr<Z6PipelineContext> ActionStage::Execute(std::unique_ptr<Z6PipelineContext> context){
    auto ext_context = dynamic_cast<Z6ExtendedPipelineContext*>(context.get());
    if (!ext_context || ext_context->has_error != 0) {
        // Skip action if context is invalid or an error occurred in previous stages
        return context;
    }

    // Parse and store Z6MoveReqJson object[cite: 1]
    try {
        Z6Root root = Z6Root::getInstance().getCopy();
        Z6Engine::getInstance().get()->move(root,ext_context->get_request() , ext_context->get_response() )
    } catch (...) {
        ext_context->has_error = 110; // Handle parsing exception if validation missed anything
    }

    return context;

}
std::unique_ptr<Z6PipelineContext> UpdateStage::Execute(std::unique_ptr<Z6PipelineContext> context){
    auto ext_context = dynamic_cast<Z6ExtendedPipelineContext*>(context.get());
    if (!ext_context || ext_context->has_error != 0) {
        // Skip action if context is invalid or an error occurred in previous stages
        return context;
    }
    auto msg = ext_context->get_response().to_json();
    Z6EngineSocketServer::getInstance().get().broadcast(msg);

}
