export default class Balls
{
    /**
     * @param {Phaser.Scene} scene
     * @param {string} ballKey
     */
    constructor(scene, ballKey)
    {
        this.scene = scene
        this.key = ballKey

        this._group = this.scene.physics.add.group()
    }

    createSprite(){
        // Create 'Heavy' animation
        this.ball.anims.create({
            key: 'Heavy',
            frames: this.ball.anims.generateFrameNames(this.key, {start: 0, end: 11, zeroPad: 4, prefix: "", suffix: ".png"}),
            frameRate: 30,
            repeat: -1              // set to (-1) to repeat forever
        }); // end of create 'Heavy' animation

        // Create 'Light' animation
        this.ball.anims.create({
            key: 'Light',
            frames: this.ball.anims.generateFrameNames(this.key, {start: 12, end: 23, zeroPad: 4, prefix: "", suffix: ".png"}),
            frameRate: 30,
            repeat: -1              // set to (-1) to repeat forever
        }); // end of create 'Light' animation

        // Create 'Reg' animation
        this.ball.anims.create({
            key: 'Reg',
            frames: this.ball.anims.generateFrameNames(this.key, {start: 24, end: 35, zeroPad: 4, prefix: "", suffix: ".png"}),
            frameRate: 30,
            repeat: -1              // set to (-1) to repeat forever
        }); // end of create 'Reg' animation
    }

    /**
     * @returns {Phaser.Physics.Arcade.Group}
     */
    get group()
    {
        return this._group
    }

    /**
     * Creates a ball object with default settings
     * @param {number} x
     * @param {number} y
     * @return {Phaser.GameObjects.Sprite}
     */
    createStandardBall(x = 500, y = 350)
    {
        this.ball = this.scene.add.sprite(x, y, this.key).setScale(0.85)
        this.createSprite()
        this.ball.play('Reg')
        this._group.add(this.ball)

        this.ball.setData('lock', false)

        let radius = this.ball.body.sourceHeight / 2
        this.ball.body.setCircle(radius)

        // These values are not tested and will very likely need to be modified
        // as I have no idea what they're like yet.
        this.ball.body.setCollideWorldBounds(true)
        this.ball.body.setBounce(.5, .5)
        this.ball.body.setMass(50)
        this.ball.body.setFriction(.1, .1)

        return this.ball
    }

    inGoal(goal, ball, xMaxSpeed = 0, yMaxSpeed = 0)
    {
        // Line needed here: if (check that ball is inside the goal)
        return ball.speedX < xMaxSpeed && ball.speedY < yMaxSpeed;
    }

}