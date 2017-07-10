#chkconfig:2345 80 90
#description:auto_run

start()
{
    echo "start MHJDeviceServer"
    /server/MHJDeviceServer &
    exit 0;
}
stop()
{

    echo -n "stop MHJDeviceServer"
    if pkill MHJDeviceServer
        then

        echo  "              [ok]"

    else

        echo  "              [failed]"

    fi

}

case "$1" in
start)
    start
    ;;
stop)
    stop
    ;;
restart)
    stop
    start
    ;;
*)
    echo "usage: $0 start|stop|restart"
    exit 0;
esac
