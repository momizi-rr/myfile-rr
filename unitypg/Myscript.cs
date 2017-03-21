using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Myscript : MonoBehaviour {
    private float num;
    private CharacterController controller;
    private Vector3 moveDirection;

	// Use this for initialization
	void Start () {
        num = 1.0f;
        controller = GetComponent<CharacterController>(); 
	}
	
	// Update is called once per frame
	void Update () {
        Quaternion k = Quaternion.Euler(num, num, num);
        transform.rotation = k;
        num++;
        
        if (Input.GetKey("space"))
        {
            moveDirection.y = 10;
        }
        moveDirection.y -= 15 * Time.deltaTime;
        controller.Move(moveDirection * Time.deltaTime);
	}

}