## Building

From this directory, build the program with:

```sh
make
```

## Testing

After building, run the tests with:

```sh
make test
```

The test command reports failures and exits unsuccessfully if any test does not pass.

## Cleaning

Remove build artifacts with:

```sh
make clean
```

## Module Diagram

```mermaid
graph TD;
    Program-->main_module;
    main_module-->reading_module;
    main_module-->stats_module;
    main_module-->histogram_module;

    subgraph reading_module["reading"]
        reading_read["reading_read()"]
    end

    subgraph stats_module["stats"]
        stats_min["stats_min()"]
        stats_max["stats_max()"]
        stats_mean["stats_mean()"]
        stats_longest_run_above["stats_longest_run_above()"]
        max_from["max_from()"]
        stats_max-->max_from
    end

    subgraph histogram_module["histogram"]
        histogram_print["histogram_print()"]
    end

    subgraph main_module["main"]
        main_function["main()"]
    end

    reading_module-->reading_read
    stats_module-->stats_min;
    stats_module-->stats_max;
    stats_module-->stats_mean;
    stats_module-->stats_longest_run_above;
    histogram_module-->histogram_print;
    main_module-->main_function;
```
