#!/usr/bin/sh

LIST_FILE=$1
REGION=$2
COUNTRY=$3
DAMNSERVER=$4
QUEUE_FILE="/pool/$DAMNSERVER/akamai_config/$REGION/akamaizer.$REGION.$DAMNSERVER.queue"
ADDING_LOCK_FILE="/pool/$DAMNSERVER/akamai_config/$REGION/akamaizer.$REGION.$DAMNSERVER.adding.lck"
LOCK_FILE="/pool/$DAMNSERVER/akamai_config/$REGION/akamaizer.$REGION.$DAMNSERVER.lck"
PARAM_FILE="/pool/$DAMNSERVER/akamai_config/$REGION/aka.$REGION.$DAMNSERVER.conf"
AKA_LOCK_FILE="/pool/$DAMNSERVER/akamai_config/$REGION/.aka_lock_file.lck"
if test ! -f "$LOCK_FILE"
then
		touch $LOCK_FILE
		more $LIST_FILE >> $QUEUE_FILE
		while test -s "$QUEUE_FILE"
		do
				COMMAND_LINE=`head -1 $QUEUE_FILE`
				while test -f "$ADDING_LOCK_FILE"
				do
				echo "FILES ARE ADDED TO YOUR QUEUE"
				echo "Hold On... :-)"
				sleep 20
				done
				sed '1 d' $QUEUE_FILE > temp_file.$REGION
				mv temp_file.$REGION $QUEUE_FILE
    			# To cheat the Akamai Lock File
    			##############################
    			while test -f "$AKA_LOCK_FILE"
    			do
					sleep 1
    			done
				#akamaize.pl -config $PARAM_FILE --inplace akabak $COMMAND_LINE
				case $COMMAND_LINE in 
					dir)  echo "$COMMAND_LINE"; for i in `ls`; do if test -d $i -a ! -h $i; then akamaize.pl -config $PARAM_FILE --recursive --inplace akabak $i; fi; done ; akamaize.pl -config $PARAM_FILE --inplace akabak *.htm*;;
					*)    akamaize.pl -config $PARAM_FILE --inplace akabak $COMMAND_LINE;;
				esac
		done
		rm $LOCK_FILE
else
		touch $ADDING_LOCK_FILE
		sleep 20
		more $LIST_FILE >> $QUEUE_FILE
		echo "AKAMAI PROCESS ALREADY RUNNING"
		echo "Your files have been added to the queue..."
		rm $ADDING_LOCK_FILE
		exit 
fi
