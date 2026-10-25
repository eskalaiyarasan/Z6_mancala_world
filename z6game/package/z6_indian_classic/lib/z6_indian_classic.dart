
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

class Z6IndianClassicPit extends StatefulWidget {
  final int index;
  final int initialValue;
  final int side;
  final bool initialActive;
  final bool turn;
  final Z6IndianClassicSideState board;
  Z6IndianClassicPit({
    super.key,
    required this.index,
    required this.initialValue,
    required this.side,
    required this.initialActive,
    required this.turn,
    required this.board,
  });

  @override
  State<Z6IndianClassicPit> createState() => Z6IndianClassicPitState();
}

class Z6IndianClassicPitState extends State<Z6IndianClassicPit> {
  late final Z6Pit _pitModel;
  final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;
  @override
  void initState() {
    super.initState();
    // Initialize the Z6Pit domain model inside the constructor/init state using the passed inputs
    _pitModel = Z6Pit(
      index: widget.index,
      value: widget.initialValue,
      active: widget.initialActive,
    );
    widget.board.register(widget.side , this);
  }

  // Delegate actions to the underlying Z6Pit model and refresh UI
  void inc() async {
    setState(() {
      _pitModel.inc();
      await theme.sound.playDrop(); 
    });
  }
  int getValue(){
      if(_pitModel.active){
        return _pitModel.value;
      }
    return -1;
  }
  int scoop() {
    int scoopedCount = -1;
    setState(() {
      scoopedCount = _pitModel.scoop();
      await theme.sound.playScoop();
    });
    return scoopedCount;
  }

  int roundUpdate(int data) async{
    int ret = -1;
    setState(() {
      ret = _pitModel.reset(data);
      await theme.sound.playOther();
    });
    return ret;
  }

  @override
  Widget build(BuildContext context) {
    // Get the current theme layout using Provider[cite: 2, 3]

    return Stack(
      children: [
        Center(
          child: _pitModel.active 
              ? theme.pit.normal(_pitModel.value) ?? Container() 
              : theme.pit.inactive()  ?? Container(),
        ),
        Positioned.fill(
          child: Material(
            color: Colors.transparent,
            child: InkWell(
              borderRadius: BorderRadius.circular(50),
              onTap: () {
                if ( (widget.board != null) && (widget.board.turn) &&(_pitModel.value > 0) ){
                  widget.board.onPitTap!(_pitModel.index);
                }
              },
            ),
          ),
        ),
      ],
    );
  }
}

class Z6IndianClassicStore extends StatefulWidget {
    final int side;
    Z6IndianClassicStore({
      super.key,
      required this.side,
      });
  @override
  State<Z6IndianClassicStore> createState() => Z6IndianClassicStoreState();
}


class Z6IndianClassicStoreState extends State<Z6IndianClassicStore> {
  late final Z6Store _model;

  @override
  void initstate() {
    super.initState();
    _model = Z6Store(side : widget.side);
  };

  void store(int v){
      setState(() {
      _model.store(v);
    });
  }

  int retrive(){
    int seeds=0;
      setState(() {
       seeds = _model.retrive();
    });
    return seeds;
  }
  
  @override
  Widget build(BuildContext context) {
    // Get the current theme layout using Provider[cite: 2, 3]
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;
    return  
        Center(
          child: theme.pit.store(_model.value) ?? Container() 
        );
  }
}

class Z6IndianClassicSide extends StatefulWidget {
  final int n;
  final int m;
  final int side;    
  final int pasu;
  final Z6IndianClassicBoard board;

  Z6IndianClassicSide({
    super.key,
    required this.n,
    required this.m,
    required this.side,
    required this.pasu,
    required this.board
  @override
  State<Z6IndianClassicSide> createState() => Z6IndianClassicSideState();
}

class Z6IndianClassicSideState extends State<Z6IndianClassicSide> {
  late bool turn;
  late Z6indianClassicStore _store;
  late List<Z6IndianClassicPit> _pitWidgets;
  // Keep track of active child pit states
  final Map<int, Z6IndianClassicPitState> _pitStates = {};
  
  final themeSpeed = Provider.of<Z6ThemeProvider>(context).currentSpeed;

  void register(int index, Z6IndianClassicPitState pitState) {
    _pitStates[index] = pitState;
  }

  void store(int seeds){
      _store.store(seeds);
  }

  void onPitTap(int index){
    if( turn){
      turn = false;
      widget.board.onPitTap(widget.side, index);
    }
  }
  void collect_pasu() async{
    int seeds = 0;
    if( widget.pasu > 0){
      for(int j= si;j < widget.n; j++){
        if(_pitStates[j].getValue() == widget.pasu){
          await  seeds = _pitStates[j].scoop()
          _store.store(seeds); 
        }
      }
    }
  }

  int sow(int seed , int si) async{
    bool ret = true;
    bool capture = false;
    bool first = true;
    if( (si < 0) || (s >= widget.n)){
        return seed;
    }
    for(int j= si;j < widget.n; j++){
      if(_pitStates[j].getValue() == -1)
          continue;
      if(seed == 0){
          seed = _pitStates[j].scoop();
          if (seed == 0){
            await widget.board.capture(widget.side , j);
            capture = true;
            break;
          }
      }else{
          seed --;
          _pitStates[k].inc();
          await Future.delayed(Duration(milliseconds:themeSpeed.speed() ));
          if( _pitStates[k] == widget.pasu){
              widget.board.capture2x = true;
              widget.board.capture2xIndex = k;
              widget.board.capture2xSide = widget.side;
          } else if (capture2x){
            widget.board.capture2x = false;
            widget.board.capture2xIndex = -1;
          }
      }
      if(first) {
        first = false;
        if(widget.pasu > 0){
          await widget.board.collect_pasu(widget.side);
        }
      }
    }
    return seed;
  }
  int front(int j){
      if((j >=0) && (j < widget.n -1) ) {
        for(int i=j+1; i< widget.n; i ++){
            if(_pitStates[j].getValue() >= 0)
                return i;
        }
    } 
      return -1; //out of this side           
  }



  int scoop(int j){
      if((j >=0) && (j < widget.n) ) {
        return _pitStates[j].scoop();
      }
      return -1;
  }
      

  @override
  void initstate() {
    super.initState();
    turn = false;
    _store = Z6IndianClassicStore( side: widget.side ); 
    var isEven = (widget.side %2 == 0)? true:false;
    _pitWidgets = List.generate(
      widget.n,
      (j) => Z6IndianClassicPit( 
        index: (isEven)? j: widget.n -j -1,
        side: widget.side,
        initialValue: widget.m,
        initialActive: true,
        turn: turn,
        board: this,
      ));
       
    widget.board.register(widget.side , this);
  };
  bool checkValidPits(){
      bool roundup = true;
      for(int j= 0;j < widget.n; j++){
          if( _pitStates[j].getValue() > 0){
              roundup = false;
              break;
          }
      }
      return roundup; 
  }
  void roundTransit(){
      int hand = _store.retrive();
      for(int j= 0;j < widget.n; j++){
          if( _pitStates[j].getValue() > 0){
              _store.store( _pitStates[j].scoop() );
          }
      }

      int hand = _store.retrive();
      for(int j= 0;j < widget.n; j++){
          hand  = _pitStates[j].roundUpdate(hand);
      }
      if(hand > 0) 
          _store.store(hand);     
  }
  void setTurn( ) async{
      // check if round up
      bool roundup = checkValidPits();
      if(roundup){
        await widget.board.roundTransit();
        if(  checkValidPits() )
          await widget.board.gameover(widget.side);
          return;
      }
      setState( () {
       turn = true;
      });
  }

  @override
  Widget build(BuildContext context) {
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;
    return Scaffold(
      appBar: AppBar(title: Text(widget.side.toString(),style:theme.color?.text ?? Color.Black)),
      body:Container(
          padding: const EdgeInsets.all(16),
          decoration: BoxDecoration(
          color: (_turn)
                  ? theme.color?.empty
                  : theme.color?.fill,          // Background color inside the container
          borderRadius: BorderRadius.circular(20), // Optional rounded corners
          border: Border.all(
            color: Colors.brown[800]!,     // Color of the thick border
            width: 4.0,                    // Thickness of the border (adjust as needed)
          ),
        ),  
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: [
            ..._pitWidgets,
            _store,
          ]
        ),
    ));
  }
}

