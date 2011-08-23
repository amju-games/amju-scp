# MakeLevel.pl
# Create a rough level file for amju super golf.
# The level file can then be tweaked in scene editor.
# Make the courses full of game objects and puzzles,
# to distinguish this game from non-puzzle golf games.
#

$GO = "game_objects";
$END = "end";
$LEVEL = 1;
$ROOM = 0;
$ID = 5200;
$PLATFORM = 0;
$TREES = 0;


##################
do_it();
##################


sub do_it
{
  print("// auto-generated level file\n// version\n11\n");
 
  # make special workers:
  # make ball worker 
#  push(@workers, ["ball"]);

  # make player character worker
#  push(@workers, ["characters", "1", "per"]);

  # Add workers for each room.
  $num_rooms = 18;
  for ($room = 0; $room < $num_rooms; $room++)
  {
    add_room();
  }   

  # print the workers.
 
  $num_workers = scalar(@workers);

  print("// num workers\n$num_workers\n");

  for ($this_worker = 0; $this_worker < $num_workers; $this_worker++)
  {
    print("// worker $this_worker\n");
    for ($this_line = 0; $this_line < 40; $this_line++)
    {
      $item = $workers[$this_worker][$this_line];
      if ($item ne "")
      {
        print $item, "\n";
      }
    }
  }
}

sub add_room()
{
  $room_file = "level-$LEVEL-room-$ROOM.room";
  $scene_file = "level-$LEVEL-room-$ROOM.comp"; 

#  push(@workers, ["add_room", "1", "$room_file"]);

  # Create the room file.
  if (open(OUTFILE, ">$room_file"))
  {
    print OUTFILE ("// $room_file\n// version\n11\nnormal.lite\n$scene_file\n1\n0\n0\n");
  }

  # Create the static scene file.
  if (open(OUTFILE, ">$scene_file"))
  {
    print OUTFILE ("// $scene_file\n//version\n11\n// num comps\n1\noutfloor1.leaf\n0\n0\n0\n0\n0\n0\n");
  }


  # Add level workers which add content to this room.
  # Content to add:
  # water
  # animals
  # tee, hole
  # rotating platforms
  # moving platforms
  # switches, to open blocked doors etc
  # blockers
  # exploding sheds
  # static sheds (animals run between them)
  # power ups: 
  #  - more power
  #  - control
  #  - turn on trajectory
  #  - bonuses to get extra lives
  #  - bonus points, to get new outfit ?
  # fire
  # trees
  # stargates
 
  #add_water();  
  #add_animal();
  #add_tee();
  #add_hole();
  #add_rotating_platform();
  #add_rotating_platform();
  #add_rotating_platform();
  #add_moving_platform("x");
  #add_moving_platform("y");
  #add_moving_platform("z");
  #add_moving_platform("x");
  #add_moving_platform("y");
  #add_moving_platform("z");
  #add_moving_platform("x");
  #add_moving_platform("y");
  #add_moving_platform("z");
  add_switch_and_blocker();
  add_switch_and_blocker();
  add_switch_and_blocker();
  #add_exploding_shed();
  ##add_fire();
  #add_trees();
  #add_stargate();

  # Add shed containing animals, with bonus lives inside them.
  #add_bonuses();

  #add_heart_bonuses();

  $ROOM++;
}

sub add_water()
{
  $depth = 4.0;
  $x_size = 10.0;
  $z_size = 5.0;
  $x = 20;
  $y = 1;
  $z = 20; 
  push(@workers, [$GO, $ID, "water", $ROOM, $x, $x + $x_size, $y, $y - $depth, $z, $z + $z_size, $END]);

  $ID++;
}

sub add_animal
{
  $mesh = $_[0];
  # Mesh name is parameter to this function.
  
  $x = 10;
  $y = 1.0;
  $z = 10;

  push(@workers, [$GO, $ID, "stupid", $ROOM, "1", $mesh, $x, $y, $z, "0", "0", "0", "// initial state", "0", "// damage", "0", "coll_hex2.leaf", "1", "0.5", "0.6", "0", "0.5", "0", "90", "0", "// hidden ID", $ID + 1, $END]);

  $ID++;
}

sub add_tee()
{
  $x = 2;
  $y = 0;
  $z = 2;

  push(@workers, [$GO, $ID, "golf_tee", $ROOM, $x, $y, $z, $END]);

  $ID++;
}

sub add_hole()
{
  $x = 20;
  $y = 0;
  $z = -20;
  
  push(@workers, [$GO, $ID, "golf_hole", $ROOM, $x, $y, $z, $END]);

  $ID++;
}

sub add_rotating_platform()
{
  $platform_file = "rplat-level-$LEVEL-room-$ROOM-$PLATFORM.move";
 
  # Create the platform file.
  if (open(OUTFILE, ">$platform_file"))
  {
    $y = 1;
    print OUTFILE "// $platform_file\n// version\n11\n// solid\noct8_2.comp\n10\n$y\n10\n0\n0\n0\nrotate\n0\n90\n0\n"; 
  }

  $PLATFORM++;

  push(@workers, [$GO, $ID, "platform", $ROOM, $platform_file, "1", $END]);

  $ID++;
}

sub add_moving_platform()
{
  # parameter passed to function is "x", "y" or "z".
  # This tells us which axis the platform moves along.
  # Parameters passed to function are in the array @_.
  # So $_[0] is the zeroth element.

  $axis = $_[0];

  $platform_file = "plat-level-$LEVEL-room-$ROOM-$PLATFORM.move";
 
  # Create the platform file.
  if (open(OUTFILE, ">$platform_file"))
  {
    if ($axis eq "x")
    {
      $z = -10;
      print OUTFILE "// $platform_file\n// version\n11\n// solid\nblock1.comp\n10\n3\n$z\n0\n0\n0\nshm_move\n8.0\n0\n-15.0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n\n"; 
    }
    elsif ($axis eq "y")
    {
      $z = -10;
      print OUTFILE "// $platform_file\n// version\n11\n// solid\nblock1.comp\n10\n3\n$z\n0\n0\n0\nshm_move\n0\n0\n0\n0\n8.0\n0\n-15.0\n0\n0\n0\n0\n0\n\n"; 
    }
    elsif ($axis eq "z")
    {
      $x = -10;
      print OUTFILE "// $platform_file\n// version\n11\n// solid\nblock1.comp\n$x\n3\n-10\n0\n0\n0\nshm_move\n0\n0\n0\n0\n0\n0\n0\n0\n8.0\n0\n-15.0\n0\n\n"; 
    }
    else
    {
      die ("Bad parameter passed to add_moving_platform.\n");
    }
  }

  $PLATFORM++;

  push(@workers, [$GO, $ID, "platform", $ROOM, $platform_file, "1", $END]);

  $ID++;
}

sub add_switch_and_timed_blocker()
{
  # Create the  switch game object - it'd a melon, to indicate there's a timer.
  $x = -6;
  $y = 0;
  $z = 0;
  push(@workers, [$GO, $ID, "change_button", $ROOM, "2", "melon.move", $x, $y, $z, "0", "0", "0", "null.comp", $x, $y, $z, "0", "0", "0", "melonexplode.txt", "// message lists", 2, "// first list", 1, "move blocker", 0, $ID + 1, "// MOVE", 6, "// second list", 0, $END]); 

  $ID++;

  $platform_file = "timed-blocker-level-$LEVEL-room-$ROOM-$PLATFORM.move";
 
  # Create the platform file.
  if (open(OUTFILE, ">$platform_file"))
  {
    print OUTFILE "// $platform_file\n// version\n11\n// solid\ncube8.comp\n10\n8\n-10\n0\n0\n0\nblocker\n// before\n10\n8\n-10\n0\n0\n0\n// after\n10\n-0.1\n-10\n0\n0\n0\n// period\n4\n"; 
  }

  $PLATFORM++;
  # Create the blocker game object
  push(@workers, [$GO, $ID, "platform", $ROOM, $platform_file, "0", $END]);

  $ID++;
 
}

sub add_switch_and_blocker()
{
  $platform_file = "blocker-level-$LEVEL-room-$ROOM-$PLATFORM.move";
 
  # Create the platform file.
  if (open(OUTFILE, ">$platform_file"))
  {
    print OUTFILE "// $platform_file\n// version\n11\n// solid\ncube8.comp\n10\n8\n-10\n0\n0\n0\nblocker\n// before\n10\n8\n-10\n0\n0\n0\n// after\n10\n-0.1\n-10\n0\n0\n0\n// period\n4\n"; 
  }

  $PLATFORM++;
  # Create the blocker game object
  push(@workers, [$GO, $ID, "platform", $ROOM, $platform_file, "0", $END]);

  $ID++;

  # Create the  switch game object
  $x = -6;
  $y = 0;
  $z = 0;
  push(@workers, [$GO, $ID, "change_button", $ROOM, "2", "moon.move", $x, $y, $z, "0", "0", "0", "null.comp", $x, $y, $z, "0", "0", "0", "melonexplode.txt", "// message lists", 2, "// first list", 1, "move blocker", 0, $ID - 1, "// MOVE", 6, "// second list", 0, $END]); 

  $ID++;
}

sub add_exploding_shed()
{
  # Currently looks like an NPC because we use MD2 model for exploding effect.
  push(@workers, [$GO, $ID, "shed", $ROOM, 1, "shed", 4, 0, 0, 0, 0, 0, "// state", 0, "// damage", 0, "// collision vol", "null.comp", "// spheres", 1, 1.5, 1.0, 0, "// shadow", 2.0, "// hidden IDs", 0, $END]);

  $ID++;
}

sub add_fire()
{
  $x = 0;
  $y = 0;
  $z = 10;
  push(@workers, [$GO, $ID, "fire", $ROOM, "fire1.fire", $x, $y, $z, 0, 0, 0, $END]);

  $ID++;
}

sub add_trees()
{
  # Create a composite file containing trees.
  # Add a sort scene game object, giving the composite file name.
  $comp_name = "level-$LEVEL-room-$ROOM-trees-$TREES.comp";
  $TREES++;
 
  # Have inner and outer comp file, so it's easy to change the overall
  # position of the group of trees.
 
  if (open(OUTFILE, ">$comp_name"))
  {
    $comp_name_2 = "level-$LEVEL-room-$ROOM-trees-$TREES-in.comp"; 
    print OUTFILE ("11\n1\n$comp_name_2\n0\n0\n0\n0\n0\n0\n");

    open(OUTFILE2, ">$comp_name_2");
    print OUTFILE2 ("11\n");
    # number of trees
    @tree_types = ("tree4_red.comp", "tree_yellow.comp", "tree_blue.comp");
    $num_types = scalar(@tree_types);
    $num_each_type = 5;
    $num_trees = $num_types * $num_each_type;

    print OUTFILE2 ("$num_trees\n");

    $yrot = 0;
    # for each type of tree, how many, and their positions
    for ($this_type = 0; $this_type < $num_types; $this_type++)
    {
      for ($one_tree = 0; $one_tree < $num_each_type; $one_tree++)
      {
        print OUTFILE2 ("$tree_types[$this_type]\n");
        # Get position
        $x = 6 * (int(rand(10)) - 5);
        $y = 0;
        $z = 6 * (int(rand(10)) - 5);
        print OUTFILE2 ("$x\n$y\n$z\n0\n$yrot\n0\n");

        $yrot += 7;
      } 
    }
  }

  push(@workers, [$GO, $ID, "sortscene", $ROOM, 1, $comp_name, $END]);

  $ID++;

}

sub add_stargate()
{
}

sub add_heart_bonuses()
{
  $y = 0;
  $x = 0;
  $z = 0;
 
 push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-3.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-2.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-1.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;
  $z += 2;

 push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-3.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-2.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-1.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;
  $z += 2;

 push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-3.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-2.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-1.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;
  $z += 2;

 push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-3.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-2.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-1.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;
  $z += 2;

 push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-3.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-2.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-1.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;
  $z += 2;

 push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-3.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-2.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-1.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;
  $z += 2;

 push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-3.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  push(@workers, [$GO, $ID, "bonus", 
                  $ROOM, "heart-2.move", 
                  $x, $y, $z, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;
  $x += 2;

  #push(@workers, [$GO, $ID, "bonus", 
  #                $ROOM, "heart-1.move", 
  #                $x, $y, $z, 0, 0, 0, 
  #                "melonexplode.txt", "10", $END]);
  $ID++;
  #x += 2;
  #z += 2;



}

sub add_bonuses()
{
  # Animal hiding a timed blocker switch
  # change button (melon) operating timed blocker  hidden in animal
  #  add_animal(); # NB hidden ID points to switch
  #  add_switch_and_timed_blocker();

  # freeze bonus - freezes animals. 
  push(@workers, [$GO, $ID, "freezebonus", 
                  $ROOM, "bean-fr.move", 
                  -3, 0, 1, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;


  # Animal hiding bonus life
  add_animal("bird");
  # bonus life hidden in animal
  push(@workers, [$GO, $ID, "bonusball", 
                  $ROOM, "star.move", 
                  -5, 0, 1, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;

  # Animal hiding trajectory bean
  # NB All bonuses are extra lives now (v.1.1)
  add_animal("cat");
#  push(@workers, [$GO, $ID, "bonus", 
#                  $ROOM, "heart-3.move", 
#                  -1, 0, 1, 0, 0, 0, 
#                  "melonexplode.txt", "10", $END]);
  push(@workers, [$GO, $ID, "bonusball", 
                  $ROOM, "star.move", 
                  -5, 0, 1, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);

  $ID++;

  add_animal("fox");
#  push(@workers, [$GO, $ID, "bonus", 
#                  $ROOM, "heart-1.move", 
#                  -5, 0, 2, 0, 0, 0, 
#                  "melonexplode.txt", "10", $END]);
  push(@workers, [$GO, $ID, "bonusball", 
                  $ROOM, "star.move", 
                  -5, 0, 1, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;

  # Animal hiding power bean
  add_animal("bird");
#  push(@workers, [$GO, $ID, "bonus", 
#                  $ROOM, "heart-2.move", 
#                  -3, 0, 2, 0, 0, 0, 
#                  "melonexplode.txt", "10", $END]);
  push(@workers, [$GO, $ID, "bonusball", 
                  $ROOM, "star.move", 
                  -5, 0, 1, 0, 0, 0, 
                  "melonexplode.txt", "10", $END]);
  $ID++;

  # Shed hiding all the above animals
  # Currently looks like an NPC because we use MD2 model for exploding effect.
 
  # IDs of animals are ID - 2, ID - 4, ID - 6, ID - 8.
 
  push(@workers, [$GO, $ID, "shed", $ROOM, 1, "shed", 4, 0, 0, 0, 0, 0, "// state", 0, "// damage", 0, "// collision vol", "block1.comp", "// spheres", 1, 1.5, 1.0, 0, "// shadow", 2.0, "// hidden IDs", 4, $ID - 2, $ID - 4, $ID - 6, $ID - 8, $END]);

  $ID++; 
}



