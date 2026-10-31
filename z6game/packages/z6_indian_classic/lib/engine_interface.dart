import 'engine.dart';

import 'package:uuid/uuid.dart';

enum Z6IndianClassicEvent { start, turn, move, finish }

class Z6IndianClassicInterface {
  // Static map to store uuid (String) as key and double as value
  static final Map<String, Z6IndianClassicEngine> _map = {};
  static final Map<String, Z6IndianClassicInterface> _self = {};
  // 1. create: Adds a new entry with an initial value of 0.0
  static String create(int level, String player1, String player2) {
    String uuid = Uuid().v4();
    _map[uuid] = Z6IndianClassicEngine(
      level: level,
      player1: player1,
      player2: player2,
    );
    return uuid;
  }

  // 2. get: Retrieves the value from the map
  // Returns double? in case the uuid doesn't exist yet
  static Z6IndianClassicEngine? get(String uuid) {
    return _map[uuid];
  }

  static void destroy(String uuid) {
    if (_map.containsKey(uuid)) {
      _map.remove(uuid);
    }
  }

  static void start(String uuid, String player) {
    if (_map.containsKey(uuid)) {
      _map[uuid]?.start(player);
      if (_self.containsKey(uuid)) {
        _self[uuid]?.publish(Z6IndianClassicEvent.start);
        _self[uuid]?.publish(Z6IndianClassicEvent.turn);
      }
    }
  }

  static void move(String uuid, int side, int index) {
    if (_map.containsKey(uuid)) {
      _map[uuid]?.move(side, index);
      if (_self.containsKey(uuid)) {
        _self[uuid]?.publish(Z6IndianClassicEvent.move);
      }
      _map[uuid]?.nextTurn();
      if (_self.containsKey(uuid)) {
        bool ended = _map[uuid]?.ended ?? false;
        _self[uuid]?.publish(
          ended ? Z6IndianClassicEvent.finish : Z6IndianClassicEvent.turn,
        );
      }
    }
  }

  static void observe(
    String uuid,
    Z6IndianClassicEvent event,
    Function callback,
  ) {
    if (!_self.containsKey(uuid)) {
      _self[uuid] = Z6IndianClassicInterface(id: uuid);
    }
    _self[uuid]?.subscribe(event, callback);
  }

  String id;
  late Map<Z6IndianClassicEvent, List<Function>> _subscribers;

  Z6IndianClassicInterface({required this.id}) {
    _subscribers = {};
  }

  void publish(Z6IndianClassicEvent event) {
    if (_subscribers.containsKey(event)) {
      final listeners = _subscribers[event];
      if (listeners != null) {
        // Create a copy of the list to prevent concurrent modification errors if a listener unsubscribes during the callback
        for (var callback in List.from(listeners)) {
          // Fallback if the callback expects zero arguments
          callback();
        }
      }
    }
  }

  void subscribe(Z6IndianClassicEvent event, Function callback) {
    _subscribers.putIfAbsent(event, () => []).add(callback);
  }

  /// Unsubscribe a specific function from an event
  void unsubscribe(Z6IndianClassicEvent event, Function callback) {
    _subscribers[event]?.remove(callback);
  }
}
