from asyncio import new_event_loop

from example_interface import ExampleInterface

# Create a new proxied object
example_object = ExampleInterface.new_proxy('org.example.test', '/')


async def print_clock() -> None:
    # Use async for loop to print clock signals we receive
    async for x in example_object.clock:
        print('Got clock: ', x)


async def call_upper() -> None:
    s = 'test string'
    s_after = await example_object.upper(s)

    print('Initial string: ', s)
    print('After call: ', s_after)


async def get_hello_world() -> None:
    print('Remote property: ', await example_object.hello_world)

loop = new_event_loop()

# Always bind your tasks to a variable
task_upper = loop.create_task(call_upper())
task_clock = loop.create_task(print_clock())
task_hello_world = loop.create_task(get_hello_world())

loop.run_forever()