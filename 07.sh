#! /bin/bash
exec 2>/dev/null
option_list="-l"
option_d="-d"
if [ "$option_d" = "$1" ];
then
    for dev in `ls /sys/block`
    do
        if [ `ls /sys/block/$dev | grep device` ]
        then
            echo $dev
            for part in `ls /sys/block/$dev | grep $dev`
            do
                echo "+--$part"
            done
            echo ""
        fi
    done
elif [ "$option_list" = "$1" ];
then
    for dev in `ls /sys/block`
    do
        if [ `ls /sys/block/$dev | grep device` ]
        then
            let "dev_size=`cat /sys/block/$dev/size`*512/1024/1024"
            is_rem="fixed"
            let "rem_val=`cat /sys/block/$dev/removable`"
            if [ $rem_val -eq 1 ]
            then
                is_rem="removable"
            fi
            echo -e "$dev\t$dev_size MiB $is_rem"
            for part in `ls /sys/block/$dev | grep $dev`
            do
                let "part_size=`cat /sys/block/$dev/$part/size`*512/1024/1024"
                is_rem="fixed"
                # let "rem_val=`cat /sys/block/$dev/$part/removable`"
                if [ -e `/sys/block/$dev/$part/removable` ] ;
                then
                    let "pat_rem_val=`cat /sys/block/$dev/$part/removable`"
                    if [ $part_rem_val -eq 1 ];
                    then
                        is_rem="removable"
                    fi
                fi
                echo "+--$part $part_size MiB $is_rem"
            done
            echo ""
        fi
    done
else
    echo -e "Usage: ./07.sh [-d | -l]\nWhere:\n\t-d - show block devices and partitions only\n\t-l - show detailed information"
fi

