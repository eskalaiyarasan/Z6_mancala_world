/// Copyright (c) 2026 Zerasix Gaming Studio LLP. All rights reserved.
/// 
/// Project: Z6 Mancala World
/// File: z6_inidan_classic.dart
/// Author:  Kalaiyarasan
/// Date:  11/10/2026
/// Package:  Z6IndianClassic
/// 
/// Notice: All information contained herein is, and remains the property of 
/// Zerasix Gaming Studio LLP. The intellectual and technical concepts contained 
/// herein are proprietary to Zerasix Gaming Studio LLP and may be covered by 
/// Indian and foreign patents or trademarks, and are protected by trade secret 
/// or copyright law. Dissemination of this information or reproduction of this 
/// material is strictly forbidden unless prior written permission is obtained 
/// from Zerasix Gaming Studio LLP.

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:z6_theme/theme_template.dart';
import 'package:z6_theme/theme_selection_widget.dart';
import 'package:z6_base/base_template.dart'
import 'dart:math';
import  'z6_indian_classic_level.dart'
import  'z6_indian_classic.dart'

  
class Z6IndianClassicBoard extends StatefulWidget {
  final int level;
  final String player1;
  final String player2;

  Z6IndianClassicSide({
    super.key,
    required this.level,
    required this.player1,
    required this.player2,
  });
  @override
  State<Z6IndianClassicSide> createState() => Z6IndianClassicSideState();
}
class Z6IndianClassicBoardState extends State<Z6IndianClassicBoard> {
  late List<Z6IndianClassicSide> _sideWidgets;
  final Map<int, Z6IndianClassicSideState> _side = {};
  final Map<int, String> _player={};
  final List<String> _history=[];
  late int turn;
  late bool started;
  late bool ended;
  late int winside;
  late bool captured;
  bool capture2x=false;
  late int capture2xIndex;
  late int capture2xSide ; 
  late int currentRound;
  late int sequence = 0;

  void register(int index, Z6IndianClassicSideState _state) {
    _side[index] = _state;
  }
  bool start(String player) async{
    if( started)
        return false; 
    if( _player[0] == player){
        started= true;
        turn = 0;
        await _side[0].setTurn();
    }
    if( _player[1] == player){
      started =true;
      turn = 1;
      await _side[1].setTurn();
    }
    return started;
    
  }
  void roundTransit() async{
    await _side[0].roundTransit();
    await _side[1].roundTransit();
    currentRound ++;
  }
  void collect_pasu(int side) async {
    side ++;
    side = side %2;
    await _side[side].collect_pasu();
  }
 
  void nextTurn( )async {
    turn ++;
    turn = turn % 2;
    captured = false;
    await _side[0].collect_pasu();
    await _side[1].collect_pasu();
    await _side[turn].setTurn();
  }
  void doubleCapture() async {
      if( capture2x){
        await int seeds = _side[capture2xSide].scoop(capture2xIndex);
        await _side[turn].store(capture2);
      }
  }
  void capture(int side , int index) async{
    captured = true;
    int next = _side[side].front(index);
    int capture2 =0;
    await doubleCapture();
    if( next < 0){
      side ++;
      side = side % 2;
      next = _side[frontside].begin();  
    }
    await int seeds = _side[side].scoop(next);
    await _side[turn].store(seeds);
  }
  void gameOver(int side) async {
      if(started && (ended == false) ) {
          ended = true;
          winside = (side +1) % 2;
      }
  }
  void onPitTap(int side , int index) async{
    int hand = 0;
    if(ended) return;
    if( turn == side ){
      sequence++;
      _history.add("$sequence: ( $side , $index)");
      while( captured  == false){
        await hand = _side[side].sow(hand, index);
        side ++;
        side = side % 2;
        index = 0;
    }
    await nextTurn();
  }
  @override
  void initstate() {
    super.initState();
    turn =0;
    started = false;
    ended = false;
    captured = false;
    capture2xIndex = -1;
    capture2xSide = 0;
    currentRound = 0;
    winside =0;
    _player[0] = widget.player1;
    _player[1] = widget.player2;
    int n = Z6IndianClassicLevel.get('n', widget.level);
    int m = Z6IndianClassicLevel.get('m',widget.level);
    int p = Z6IndianClassicLevel.get('p',widget.level);
    _sideWidgets = List.generate(
                  2,
                  (j) => Z6IndianClassicSide(
                    n: n,
                    m: m,
                    pasu: p,
                    side: j,
                    board: this,
    ));    
  }
    
  
  @override
  Widget build(BuildContext context) {
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;
    return Scaffold(
      appBar: AppBar(title: Text("Z6 Indian Classic ${widget.level}",style:theme.color?.text ?? Color.black)),
      drawer: Drawer( child: ListView(
              padding: EdgeInserts.zero,
              children[
                DrawerHeader(
                    decoration: BoxDecoration(color: style:theme.color?.fill ?? Color.blue),
                    child: Text("Round: $currentRound ", style:theme.color?.text ?? Color.Black),
                ),
                ExpansionTile(
                    leading: const Icon(Icons.history),
                    title: const Text('View History'),
                    children:[
                      for (String item in _history)
                        ListTile(
                          leading: const Icon(Icons.add),
                          title: Text(item),
                          // No onTap is provided here. The icon and text will still render normally!
                        ),
                ]),
          ])),
      body:Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: [
            if(!ended){
              ..._sideWidgets,
            }else{
              Text("Game Ended", style: TextStle( fontSize: 20,
                    fontWeight: FontWeight.bold,
                    color: Colors.redAccent,
              )),
              Text("Winner: ${_player[winside]}", style: TextStle( fontSize: 20,
                    fontWeight: FontWeight.bold,
                    color: Colors.redAccent,
              )),
            
            },
        ]),
    );
  }
}   

