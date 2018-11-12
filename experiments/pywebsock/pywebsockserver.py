import asyncio
import websockets
import time


async def hello(websocket, path):

    while True:
        time.sleep(1)
        greeting = "hello"
        await websocket.send(greeting)


start_server = websockets.serve(hello, 'localhost', 1234)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
