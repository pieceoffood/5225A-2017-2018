from sys import argv

max = int(argv[1])

for i in range(max + 1):
    print('#define NEW_ASYNC_VOID_%d(%s) \\' % (i, ', '.join(['func', *('type%d' % j for j in range(i))])))
    print('byte func##Dummy; \\')
    print('typedef struct _asyncData_##func { \\')
    if i > 0:
        for j in range(i):
            print('  type%d arg%d; \\' % (j, j))
    else:
        print('  int _dummy[0]; \\')
    print('} sAsyncData_##func; \\')
    print('void _asyncTask_##func(sAsyncData_##func *data) { \\')
    print('  writeDebugStreamLine("Instance of " #func " started"); \\')
    print('  func(%s); \\' % ', '.join('data->arg%d' % j for j in range(i)))
    print('} \\')
    print('byte func##Async(%s) { \\' % ', '.join('type%d arg%d' % (j, j) for j in range(i)))
    # print('  writeDebugStreamLine("Starting instance of " #func); \\')
    print('  sAsyncData_##func data; \\')
    for j in range(i):
        print('  data.arg%d = arg%d; \\' % (j, j))
    print('  return _startAsync(&func##Dummy, &data); \\')
    print('} \\')
    print('bool func##Kill() { \\')
    # print('  writeDebugStreamLine("Killing all instances of " #func); \\')
    print('  for (int i = 0; i < TASK_POOL_SIZE; ++i) \\')
    print('    if (gAsyncTaskData[i].id == &func##Dummy) { \\')
    print('      kill(i); \\')
    print('      return true; \\')
    print('    } \\')
    print('  return false; \\')
    print('}')
    print()

print('#define USE_ASYNC(func) \\')
print('if (data->id == &func##Dummy) { \\')
print('  _asyncTask_##func((sAsyncData_##func *)data->data); \\')
print('  notify(data->notifier); \\')
print('  return true; \\')
print('}')
print()

print('#define ASYNC_ROUTINES(content) \\')
print('bool _runAsync(sAsyncTaskData *data) { \\')
print('  writeDebugStreamLine("Starting asynchronous function on threadPoolTask%d", nCurrentTask - threadPoolTask0); \\')
print('  content \\')
print('  return false; \\')
print('}')
print()

print('typedef struct _sAsyncTaskData {')
print('  byte *id;')
print('  void *data;')
print('  sNotifier notifier;')
print('} sAsyncTaskData;')
print('sAsyncTaskData gAsyncTaskData[TASK_POOL_SIZE];')
print()

print('void await(byte index, unsigned long timeout, const string description);')
print('void kill(byte index);')
print('bool _runAsync(sAsyncTaskData *data);')
print('byte _startAsync(byte *id, void *data);')
print()

for i in range(20):
    print('#if TASK_POOL_SIZE > %d' % i)
    print('task threadPoolTask%d() {' % i)
    print('  if (!_runAsync(&gAsyncTaskData[%d]))' % i)
    print('    writeDebugStreamLine("Failed to start asynchronous function on threadPoolTask%d!");' % i)
    print('}')
    print('#endif')
    print()
