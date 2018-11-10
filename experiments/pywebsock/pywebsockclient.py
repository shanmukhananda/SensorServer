import asyncio
import websockets
import time

async def hello():
    async with websockets.connect(
            'ws://192.168.1.7:9000') as websocket:
        while True:
            greeting = await websocket.recv()
            print("len=",len(greeting))

asyncio.get_event_loop().run_until_complete(hello())
