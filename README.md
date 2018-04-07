# KL ak Kiwi Logger

This is logging library for so called structured logging. The log
records consist of pairs of keys and values. See logfmt for ideas
around structured logging. KL based on ideas of Kiwi logger for Go
but C version has simplier API and fewer functionality.

In brief library in current state offers:

- output log records like space separated pairs of key=value
  (hardcoded format)
- dynamic sinks with filtering

## Brief explanation of logging model used in Kiwi

1. Log everything in your code that possible to log.
2. Add one or more file writers (sinks) for saving log records.
3. Each sink filters that the records it wants to pass. Initially
   all the records filtered out by the sink.

## Possible future features

- more available conditions for filtering
- context for the log records like in Go Kiwi
