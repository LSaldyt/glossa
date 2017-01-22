from time import sleep
import threading

"""
Common conversions to seconds

Ideal usage:

    every 2 days:
        email('clients@gmail.com', 'Hello i am selling the things')

"""

__CONVERSION_TABLE__ = {
        'seconds' : 1.,
        'minutes' : 60.,
        'hours'   : 3600.,
        'days'    : 86164.09,
        'weeks'   : 603148.63,
        'months'  : 2412594.52
        }

def to_seconds(n, label='seconds'):
    if label not in __CONVERSION_TABLE__:
        message = '''Unknown time label %s\nValid labels are:\n%s''' % \
            (
             label, 
             '\n'.join(__CONVERSION_TABLE__.keys())
            )
        raise ValueError(message)
    return __CONVERSION_TABLE__[label] * n

def every(f, duration, durationLabel='seconds'):
    if not durationLabel.endswith('s'):
        durationLabel += 's'
    duration = to_seconds(duration, durationLabel)

    def __repeated_task__():
        while True:
            f()
            sleep(duration)

    thread = threading.Thread(target=__repeated_task__)
    thread.start()
    return thread

"""
__every__(lambda : print("Sent email"), 1, 'seconds')
print("Repeated task started, moving to other things..")
"""
