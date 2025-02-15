#!/bin/bash

# Default values
THREADS=1
RUNS=5
COUNT=10000

# Function to parse named arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -threads)
            THREADS="$2"
            shift 2
            ;;
        -runs)
            RUNS="$2"
            shift 2
            ;;
        -count)
            COUNT="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

TARGET="http://127.0.0.1:5000"
ENDPOINT="/benchmark"
NGINX_CONF="/tmp/nginx_papy.conf"
NGINX_PID="/tmp/nginx_papy.pid"

# Function to create a temporary Nginx configuration
create_nginx_config() {
    cat <<EOL > $NGINX_CONF
events {}

http {
    server {
        listen 5000;
        server_name localhost;

        location / {
            return 200 "hello flask";
            add_header Content-Type text/plain;
        }

        location /benchmark {
            return 200 "benching";
            add_header Content-Type text/plain;
        }
    }
}
EOL
}

start_nginx() {
    echo "starting nginx..."
    sudo nginx -c $NGINX_CONF -p /tmp -g "pid $NGINX_PID;"
    sleep 1  # give nginx a moment
}

stop_nginx() {
    if [[ -f $NGINX_PID ]]; then
        echo "stopping nginx..."
        sudo kill "$(cat $NGINX_PID)" 2>/dev/null
        rm -f $NGINX_PID
    fi
}

# return when nginx is ready to use
wait_for_nginx() {
    for i in {1..10}; do
        if curl -s --head --request GET "$TARGET$ENDPOINT" | grep "200 OK" > /dev/null; then
            echo "nginx is up!"
            return 0
        fi
        echo "waiting for nginx to start..."
        sleep 1
    done
    echo "ERROR: nginx did not start properly"
    exit 1
}

# run & measure Papy (number will be different from what Papy shows due to overhead of starting things. 
# You can decrease relative impact of such overhead by increasing count)
benchmark() {
    local papy_path=$1
    local total_time=0

    echo "Benchmarking $papy_path for $RUNS runs (Threads: $THREADS, Count: $COUNT)..."

    for ((i = 1; i <= RUNS; i++)); do
        START=$(date +%s%N)
        ./$papy_path --threads $THREADS --target "$TARGET" --endpoint "$ENDPOINT" --count $COUNT > /dev/null 2>&1
        END=$(date +%s%N)
        TIME=$(( (END - START) / 1000000 ))  # Convert ns to ms
        total_time=$((total_time + TIME))
        echo "Run #$i: $TIME ms"
    done

    echo "Total time for $papy_path: $total_time ms"
    echo "=================================="
}

# create and run temp nginx config
create_nginx_config
start_nginx
wait_for_nginx

echo "Starting benchmark..."
benchmark "../bin/dev/papy"
benchmark "../bin/rel/papy"

# stop nginx after benchmarking
stop_nginx
rm -f $NGINX_CONF
