
1.
google 的中间服务器

是用的 STUN  用途中。不知国内是否可用。这是个标准协议。

2.
STUN（Session Traversal Utilities for NAT，NAT会话穿越应用程序）是一种网络协议，它允许位于NAT（或多重NAT）

3.
WebRTC 实现P2P音视频通话——搭建stun/turn P2P穿透和转发服务器


4.
国内应该是用不了的。 

https://blog.csdn.net/qq_44938451/article/details/122158975


上述序列中，WebRTC并不提供Stun服务器和Signal服务器，服务器端需要自己实现。Stun服务器可以用google提供的实现stun协议的测试服务器（stun:stun.l.google.com:19302），Signal服务器则完全需要自己实现了，它需要在ClientA和ClientB之间传送彼此的SDP信息和candidate信息，ClientA和ClientB通过这些信息建立P2P连接来传送音视频数据。由于网络环境的复杂性，并不是所有的客户端之间都能够建立P2P连接，这种情况下就需要有个relay服务器做音视频数据的中转。

而Signal服务器其实就是我们的websocket，stun服务器其实就是webrtc啦。



